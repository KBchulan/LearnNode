#include "../include/MysqlDao.h"
#include "../include/LogManager.h"
#include "../include/MysqlManager.h"

MysqlManager::~MysqlManager()
{
    LOG_SQL->info("MysqlManager has been destructed");
}

bool MysqlManager::CheckPwd(const std::string &email, const std::string &pwd, UserInfo &userInfo)
{
    return _dao.CheckPasswd(email, pwd, userInfo);
}