#include "worker.h"

Worker::Worker(const ConfData& conf, Prange range)
	:_p_processT(NULL),_range(range)
{
    _sqlIns = openMysql(conf.dbip.c_str(),conf.dbport,conf.dbuser.c_str(),conf.dbpwd.c_str(),conf.dbname.c_str());
    _mcIns = openMc(conf.mcip.c_str(),conf.mcport);
}

Worker::~Worker()
{
	if( _sqlIns!=NULL )
    {
    	closeMysql(_sqlIns);
    	_sqlIns=NULL;
    }
    if( _mcIns!=NULL)
    {
       closeMc(_mcIns);
       _mcIns=NULL;
    }

}

void Worker::_proessCb(MYSQL* sqlIns ,memcached_st* mcIns, const Prange& range )
{
    if( sqlIns==NULL || mcIns==NULL )
    {
        mylogF("process callback err"); 
        return;
    }

    for(int i = range.sid ; i<=range.eid ; ++i)
    {
        char sql[SQLLEN]= {'\0'};
        snprintf(sql,SQLLEN,"select weigth from info_dynamic where pid = %d", i);
        int weight;
        doStatusSql(sqlIns,sql,weight);
        mylogD("pid:%d  ---  weight:%d", i, weight);
         char mcKey[1024]={0};
        char mcVal[1024]={0};
        snprintf(mcKey,1024,"uptW_%d",i);
        snprintf(mcVal,1024,"%d",weight);
        setMc(mcIns,mcKey,mcVal);
    }
}

bool Worker::start()
{
	if( _sqlIns==NULL || _mcIns==NULL || _p_processT!=NULL)
        return false;
	_p_processT = new std::thread(Worker::_proessCb,_sqlIns,_mcIns,_range);
    return true;
}

bool Worker::join()
{
	if(_p_processT!=NULL)
	{
    	_p_processT->join();
        delete _p_processT;
        _p_processT=NULL;
    }
}
