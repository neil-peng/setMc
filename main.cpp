#include "common.h"
#include "./lib/lightConf/ConfigFile.h"
#include "mmysql.h"
#include "worker.h"

static int GetProcessCount(const ConfData& conf)
{
	int count = 0;
	MYSQL* sqlIns = openMysql(conf.dbip.c_str(),conf.dbport,conf.dbuser.c_str(),conf.dbpwd.c_str(),conf.dbname.c_str());
	if(sqlIns == NULL)
		return -1;
	char sql[SQLLEN] = {'\0'};
	snprintf(sql,SQLLEN,"select count from info_status;");
	doStatusSql(sqlIns,sql,count);
	closeMysql(sqlIns);
	return count;

}

static ConfData loadConf()
{
	ConfData confData;
	ConfigFile conf("./conf/dp.conf");
    try
    {
    	confData.dbip = (std::string) conf.Value("DB","ip");
    	confData.dbport = (int)conf.Value("DB","port");
    	confData.dbpwd = (std::string) conf.Value("DB","pwd");
    	confData.dbuser = (std::string) conf.Value("DB","user");
    	confData.dbname = (std::string) conf.Value("DB","db");
    	confData.mcip = (std::string) conf.Value("MC","ip");
    	confData.mcport = (int) conf.Value("MC","port");
    	confData.threadNum = (int) conf.Value("common","threadNum");
    }
    catch(char const* msg)
    {
    	mylogD("load conf catch exception : %s",msg);
        exit(0);
    }
    return confData;
}



int main()
{
	ConfData conf = loadConf();
	int count = GetProcessCount(conf); 
	if(count<=0)
		return -1;
	int eachRange = count/conf.threadNum;
	uint32_t s = 1;
	uint32_t e = 0;
	Worker** worker = new Worker*[conf.threadNum];

	for (int i = 0; i < conf.threadNum; ++i)
	{	
		e = s + eachRange;
		if(e>count)
			e = count;
		worker[i] = new Worker(conf,Prange(s,e));
		mylogD("%d : %d - %d",i,s,e);
		s = e+1;
		worker[i]->start();
	}

	for (int i = 0; i < conf.threadNum; ++i)
	{
		worker[i]->join();	
	}


	for (int i = 0; i < conf.threadNum; ++i)
		delete worker[i];
	delete worker;

    return 0;
}
