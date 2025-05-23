#include "../include/MysqlDao.h"
#include "../include/LogManager.h"
#include "../include/ConfigManager.h"

SqlConnection::SqlConnection(sql::Connection *con, std::uint64_t lastTime)
    : _con(con), _last_oper_time(lastTime)
{
}

MysqlPool::MysqlPool(const std::string &url, const std::string &user, const std::string &passwd, const std::string &schema, std::uint32_t poolSize)
    : _url(url), _user(user), _passwd(passwd), _schema(schema), _poolSize(poolSize), _b_stop(false)
{
    try
    {
        for (uint32_t i = 0; i < _poolSize; i++)
        {
            sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
            auto *con = driver->connect(_url, _user, _passwd);
            con->setSchema(_schema);

            // 获取当前时间戳
            auto currentTime = std::chrono::system_clock::now().time_since_epoch();

            long long timeStamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();

            _pool.push(std::make_unique<SqlConnection>(con, timeStamp));
        }

        _check_thread = std::thread([this]
        {
            while (!_b_stop)
            {
                CheckConnection();
                std::this_thread::sleep_for(std::chrono::seconds(60));
            } 
        });
    }
    catch (const std::exception &e)
    {
        LOG_SQL->error(R"({} : {})", __FILE__, "Mysql pool init failed!");
        Close();
        throw;
    }
}

void MysqlPool::CheckConnection()
{
    std::lock_guard<std::mutex> guard(_mutex);
    int poolSize = _pool.size();

    auto currentTime = std::chrono::system_clock::now().time_since_epoch();
    long long timeStamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();

    for (uint32_t i = 0; i < poolSize; i++)
    {
        auto con = std::move(_pool.front());
        _pool.pop();

        RAII defer([this, &con]
                    { _pool.push(std::move(con)); });

        if (timeStamp - con->_last_oper_time < 600)
            continue;

        try
        {
            std::unique_ptr<sql::Statement> stmt(con->_con->createStatement());
            stmt->executeQuery("SELECT 1");
            con->_last_oper_time = timeStamp;
            LOG_SQL->info("execute timer alive query, cur is {}", con.get()->_last_oper_time);
        }
        catch (const std::exception &e)
        {
            // 检测不到心跳就重新生成一个连接
            LOG_SQL->error("execute timer alive query, cur is {}", e.what());
            sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
            auto *newCon = driver->connect(_url, _user, _passwd);
            newCon->setSchema(_schema);
            con->_con.reset(newCon);
            con->_last_oper_time = timeStamp;
        }
    }
}

std::unique_ptr<SqlConnection> MysqlPool::GetConnection()
{
    std::unique_lock<std::mutex> lock(_mutex);

    // false阻塞
    _cond.wait(lock, [this]
               {
        if(_b_stop)
            return true;
        return !_pool.empty(); });

    if (_b_stop)
        return nullptr;

    std::unique_ptr<SqlConnection> con = std::move(_pool.front());
    _pool.pop();
    return con;
}

void MysqlPool::ReturnConnection(std::unique_ptr<SqlConnection> con)
{
    std::lock_guard<std::mutex> guard(_mutex);
    if (_b_stop)
        return;

    _pool.push(std::move(con));
    _cond.notify_one();
}

void MysqlPool::Close()
{
    _b_stop = true;
    _cond.notify_all();
}

MysqlPool::~MysqlPool()
{
    if (_check_thread.joinable())
        _check_thread.join();

    std::lock_guard<std::mutex> guard(_mutex);
    while (!_pool.empty())
        _pool.pop();
}

MysqlDao::MysqlDao()
{
    auto &gCfgMgr = ConfigManager::GetInstance();
    std::string host = gCfgMgr["Mysql"]["host"];
    std::string port = gCfgMgr["Mysql"]["port"];
    std::string user = gCfgMgr["Mysql"]["user"];
    std::string passwd = gCfgMgr["Mysql"]["password"];
    std::string schema = gCfgMgr["Mysql"]["schema"];
    _pool.reset(new MysqlPool(host + ":" + port, user, passwd, schema, std::thread::hardware_concurrency()));
}

MysqlDao::~MysqlDao()
{
    _pool->Close();
}

bool MysqlDao::CheckPasswd(const std::string &email, const std::string &passwd, UserInfo &userInfo)
{
    auto con = _pool->GetConnection();

    RAII defer([this, &con]
                { _pool->ReturnConnection(std::move(con)); });

    if(con == nullptr)
        return false;

    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("SELECT * FROM user WHERE email = ?"));
        pstmt->setString(1, email);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        std::string origin_pwd = "";
        while(res->next())
        {
            origin_pwd = res->getString("passwd");
            LOG_SQL->info("check passwd, origin_pwd is {}", origin_pwd);
            break;
        }

        if(origin_pwd != passwd)
            return false;

        userInfo._passwd = origin_pwd;
        userInfo._uid = res->getInt("uid");
        userInfo._name = res->getString("name");
        userInfo._email = res->getString("email");
        return true;
    }
    catch(sql::SQLException& e)
    {
        LOG_SQL->error("SQLException: {} (MySQL error code: {}, SQLState: {})", e.what(), e.getErrorCode(), e.getSQLState());
        return false;
    }
}