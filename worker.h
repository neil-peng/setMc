#pragma once
#include <thread>
#include "mmc.h"
#include "mmysql.h"
#include "common.h"

class Worker
{
private:
	std::thread* _p_processT;
	Prange _range;
	static void _proessCb(MYSQL* sqlIns ,memcached_st* mcIns,const Prange& range );
	MYSQL* _sqlIns;
    memcached_st* _mcIns;
    Worker(Worker&){}
	Worker& operator=(Worker&){}
public:
	Worker(const ConfData& conf, Prange range);
    bool start();
	bool join();
	~Worker();
};