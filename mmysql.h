#pragma once
#include "./lib/mysql/include/mysql.h"
#include <string>
#include <vector>
#include "common.h"
typedef std::vector< std::pair<std::string,std::string > > SQLDATA ;
extern MYSQL* openMysql(const char* serv,int port,const char* user, const char* pwd,const char* db);
extern bool doStatusSql(MYSQL *conn,char* sql,int& count);
extern MYSQL* closeMysql(MYSQL* conn);
