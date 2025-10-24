#include "ActorCsp.hpp"

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <middleware/Logger.hpp>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

namespace core {

// 消息基类的设计
struct Message {
  enum class Type : std::uint8_t { LOGIN, MOVE, ATTACK, LOGOUT };
  Type type_;
  int userId_;

  Message(Type type, int userID) : type_(type), userId_(userID) {}
  virtual ~Message() {}
};

struct LoginMessage final : public Message {
  std::string userName_;

  LoginMessage(int userId, std::string user)
      : Message(Type::LOGIN, userId), userName_(std::move(user)) {}
};

struct MoveMessage final : public Message {
  int x_;
  int y_;
  int z_;
  MoveMessage(int userId, int pos_x, int pos_y, int pos_z)
      : Message(Type::MOVE, userId), x_(pos_x), y_(pos_y), z_(pos_z) {}
};

struct AttackMessage final : public Message {
  int targetId_;
  int skillId_;

  AttackMessage(int userId, int targetId, int skillId)
      : Message(Type::ATTACK, userId), targetId_(targetId), skillId_(skillId) {}
};

struct LogoutMessage final : public Message {
  LogoutMessage(int userId) : Message(Type::LOGOUT, userId) {}
};

class Actor {
protected:
  int id_;
  std::queue<std::shared_ptr<Message>> mailBox_;
  std::mutex mailBoxMutex_;
  std::condition_variable_any mailBoxCV_;
  std::atomic_bool running_;
  std::jthread workerThread_;

public:
  Actor(int actorId) : id_(actorId), running_(false) {}

  virtual ~Actor() { stop(); }

  void start() {
    running_.store(true, std::memory_order_release);
    workerThread_ = std::jthread(&Actor::processMessages, this);
  }

  void stop() {
    {
      std::unique_lock<std::mutex> lock{mailBoxMutex_};
      running_.store(false, std::memory_order_release);
    }
    mailBoxCV_.notify_all();
  }

  void send(const std::shared_ptr<Message> &message) {
    std::unique_lock<std::mutex> lock{mailBoxMutex_};
    mailBox_.push(message);
    lock.unlock();

    mailBoxCV_.notify_one();
  }

protected:
  virtual void handleMessage(std::shared_ptr<Message> message) = 0;

  void processMessages() {
    while (running_.load(std::memory_order_acquire)) {
      std::unique_lock<std::mutex> lock{mailBoxMutex_};
      mailBoxCV_.wait(lock,
                      [&]() -> bool { return !running_ || !mailBox_.empty(); });

      if (!running_ && mailBox_.empty()) {
        return;
      }

      std::shared_ptr<Message> message = mailBox_.front();
      mailBox_.pop();
      lock.unlock();

      handleMessage(message);
    }
  }
};

class PlayerActor final : public Actor {
public:
  PlayerActor(int playerId) : Actor(playerId) {}

protected:
  void handleMessage(std::shared_ptr<Message> message) override {
    switch (message->type_) {
    case Message::Type::LOGIN: {
      auto loginMsg = std::static_pointer_cast<LoginMessage>(message);
      login(loginMsg->userName_);
      break;
    }
    case Message::Type::MOVE: {
      auto moveMsg = std::static_pointer_cast<MoveMessage>(message);
      move(moveMsg->x_, moveMsg->y_, moveMsg->z_);
      break;
    }
    case Message::Type::ATTACK: {
      auto attackMsg = std::static_pointer_cast<AttackMessage>(message);
      attack(attackMsg->targetId_, attackMsg->skillId_);
      break;
    }
    case Message::Type::LOGOUT: {
      logout();
      break;
    }
    default:
      break;
    }
  }

private:
  std::string username_;
  int x_{}, y_{}, z_{};
  bool isLoginedIn_{};

  void login(const std::string &user) {
    logger.info("玩家 {} 登录: {}", id_, user);
    username_ = user;
    isLoginedIn_ = true;
  }

  void move(int newX, int newY, int newZ) {
    if (!isLoginedIn_) {
      return;
    }
    logger.info("玩家 {} 移动到位置: ({}, {}, {})", id_, newX, newY, newZ);
    x_ = newX;
    y_ = newY;
    z_ = newZ;
  }

  void attack(int targetId, int skillId) {
    if (!isLoginedIn_) {
      return;
    }
    logger.info("玩家 {} 使用技能 {} 攻击玩家 {}", id_, skillId, targetId);
  }

  void logout() {
    if (!isLoginedIn_) {
      return;
    }
    logger.info("玩家 {} 登出", id_);
    isLoginedIn_ = false;
  }
};

class GameWorld {
private:
  std::unordered_map<int, std::shared_ptr<PlayerActor>> players_;
  std::mutex playersMutex_;

public:
  std::shared_ptr<PlayerActor> getOrCreatePlayer(int playerId) {
    std::unique_lock<std::mutex> lock(playersMutex_);
    auto iter = players_.find(playerId);
    if (iter != players_.end()) {
      return iter->second;
    }

    auto player = std::make_shared<PlayerActor>(playerId);
    player->start();
    players_[playerId] = player;
    return player;
  }

  void shutdown() {
    std::unique_lock<std::mutex> lock(playersMutex_);
    for (auto &pair : players_) {
      pair.second->stop();
    }
    players_.clear();
  }
};

template <typename T> class Channel {
public:
  Channel(size_t capacity = 0) : capacity_(capacity) {}

  bool send(T value) {
    std::unique_lock<std::mutex> lock{mtx_};
    cv_producer_.wait(lock, [&]() -> bool {
      return (capacity_ == 0 && queue_.empty()) || queue_.size() < capacity_ ||
             !running_;
    });

    if (!running_) {
      return false;
    }

    queue_.push(value);
    cv_consumer_.notify_one();
    return true;
  }

  bool receive(T &value) {
    std::unique_lock<std::mutex> lock{mtx_};
    cv_consumer_.wait(lock,
                      [&]() -> bool { return !queue_.empty() || !running_; });

    if (!running_ && queue_.empty()) {
      return false;
    }

    value = queue_.front();
    queue_.pop();
    cv_producer_.notify_one();
    return true;
  }

  void close() {
    {
      std::unique_lock<std::mutex> lock{mtx_};
      running_.store(false, std::memory_order_release);
    }
    cv_producer_.notify_all();
    cv_consumer_.notify_all();
  }

private:
  std::atomic_bool running_{true};
  std::queue<T> queue_;
  std::mutex mtx_;
  std::condition_variable_any cv_producer_;
  std::condition_variable_any cv_consumer_;
  size_t capacity_;
};

void ActorCsp::enterFunc() noexcept {
  // actorDesign();

  cspDesign();
}

void ActorCsp::actorDesign() noexcept {
  /*
    Actor模型是一种并发计算模型，它将计算单元视为封装的、相互独立的实体(即"actor")，这些实体通过消息队列传递而非共享状态来进行通信。
    一个actor维护一个消息队列，接收并缓存别的actor传递来的消息，同一时间只能处理一个消息，在处理消息时仍然可以接收其他actor
    传来的消息，不同的actor是通过消息传递的方式进行通信的(即操作彼此的队列)

    eg:
    asio的笔记里我们有一个逻辑线程，初始化一个IOcontextPool，每个核分配一个iocontext，用于监听，每个线程接收到数据并封装
    为一个数据包后投递到我们的逻辑队列，这个投递肯定是线程安全的，然后由逻辑线程进行消费，这里的逻辑线程就是我们的actor，而这个
    逻辑队列就是我们的消息队列
  */
  GameWorld gameWorld;
  auto player1 = gameWorld.getOrCreatePlayer(1);
  auto player2 = gameWorld.getOrCreatePlayer(2);
  player1->send(std::make_shared<LoginMessage>(1, "张三"));
  player2->send(std::make_shared<LoginMessage>(2, "李四"));
  player1->send(std::make_shared<MoveMessage>(1, 100, 200, 300));
  player2->send(std::make_shared<MoveMessage>(2, 100, 200, 300));
  player1->send(std::make_shared<AttackMessage>(1, 2, 1));
  player2->send(std::make_shared<AttackMessage>(2, 1, 1));
  player1->send(std::make_shared<LogoutMessage>(1));
  player2->send(std::make_shared<LogoutMessage>(2));
  std::this_thread::sleep_for(std::chrono::seconds(1));
  gameWorld.shutdown();
}

void ActorCsp::cspDesign() noexcept {
  /*
    Csp模型和Actor比较类似，但是不一样的地方在于发送者是将消息发送到一个Channel中，而不关注谁来取出这个消息，即类似于漂流瓶
    两个独立的并发实体是通过共享的channel进行通信，整体来说是1: N
  */
  Channel<int> chan(10); // 10缓冲的channel

  std::thread producer([&]() {
    for (int i = 0; i < 5; ++i) {
      chan.send(i);
      logger.info("Sent: {}", i);
    }
    chan.close();
  });

  std::thread consumer([&]() {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(500));
    int val;
    while (chan.receive(val)) {
      logger.info("Received: {}", val);
    }
  });

  producer.join();
  consumer.join();
}

} // namespace core