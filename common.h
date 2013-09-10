#pragma once
#include <unistd.h>
#include <string>
#include <vector>

#define mylog(format,args...) do{\
    fprintf(stdout,format"\n",##args);\
}while(0)

#define mylogD(format,args...) do{\
    fprintf(stdout,"DEBUG: "format"\n",##args);\
}while(0)

#define mylogS(format,args...) do{\
        fprintf(stdout,format,##args);\
}while(0)


#define mylogF(format,args...) do{\
    fprintf(stdout,"FATAL: "format"\n",##args);\
}while(0)

#define mylogW(format,args...) do{\
    fprintf(stdout,"WARNNING: "format"\n",##args);\
}while(0)


#define SQLLEN 2048

struct Prange
{
    uint32_t sid;
    uint32_t eid;
    Prange(){}
    Prange(uint32_t s, uint32_t e)
        :sid(s),eid(e){}
    
};

struct ConfData
{
    //for store
    std::string dbip;
    int dbport;
    std::string dbuser;
    std::string dbpwd;
    std::string dbname;
    std::string mcip;
    int mcport;
    int pidCount;
    int threadNum;

};

