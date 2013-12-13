#pragma once

#include <map>
#include <string>
#include <list>

#include <iostream>
#include "hash.h"
struct SQL_PARAM_INFO
{
	std::string param;
	int pos;
};
typedef std::map<std::string , std::string> SQL_PARAM;
typedef std::list<SQL_PARAM_INFO> SQL_PARAM_INFO_LIST;
typedef std::map<HASH, SQL_PARAM_INFO_LIST> SQL_CACHE;
 
typedef SQL_PARAM_INFO_LIST::iterator SQL_PARAM_INFO_IT;
typedef SQL_CACHE::iterator SQL_CACHE_IT;
 
 