#ifndef MYSQLMANAGER_H
#define MYSQLMANAGER_H

#include "const.h"
#include "MysqlDao.h"

class MysqlDao;
class MysqlManager final : public Singleton<MysqlManager>
{
    friend class Singleton<MysqlManager>;

public:
    ~MysqlManager();
    
    // 验证用户密码是否正确,并返回用户信息
    bool CheckPwd(const std::string &email, const std::string &pwd, UserInfo &userInfo);

private:
    MysqlManager() = default;

private:
    MysqlDao _dao;
};

#endif // !MYSQLMANAGER_H