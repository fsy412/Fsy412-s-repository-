#include "SqlParse.h"
#include "log.hpp"
extern SQL_CACHE SQL_cache;
extern module::CLog *log;
CSQLParse::CSQLParse(void)
{
}

CSQLParse::~CSQLParse(void)
{
}

void CSQLParse::Parse( const std::string& sql, HASH hash )
{
	std::cout<<"parse "<< sql<<std::endl;
	SQL_PARAM_INFO_LIST sql_param_info_list;
	std::string::size_type i = sql.find_first_of("$");
	std::list<int> pos_list;
	while(i != std::string::npos)
	{				
		std::string::size_type left = sql.find_first_of("{", i);
		std::string::size_type right = sql.find_first_of("}", i);
		std::string param = sql.substr(left + 1,right - left - 1 );
		SQL_PARAM_INFO sql_parm;
		sql_parm.param = sql.substr(left + 1,right - left - 1 );
		sql_parm.pos = i;
		sql_param_info_list.push_back(sql_parm);
		i = sql.find_first_of("$",i + 1);
	}
	SQL_cache[hash] = sql_param_info_list;
}

std::string CSQLParse::GetSQL(const std::string& SQL,SQL_PARAM & sql_param )
{
	std::string result;
	HASH hash;
	hash = Hash(SQL.c_str());
	result = SQL;
	if (SQL_cache.find(hash) != SQL_cache.end())
	{
		SetParameter(sql_param, hash, result);
		std::cout<<"SQL exists in Cache return result" <<std::endl;
	}else
	{
		Parse(SQL, hash);
		SetParameter(sql_param, hash, result);
	}
	return result;
}

void CSQLParse::SetParameter( SQL_PARAM & sql_param, HASH hash, std::string &result )
{
 	SQL_CACHE_IT sql_cache_it = SQL_cache.find(hash);
	if (sql_cache_it == SQL_cache.end())
	{
		//error
	}
	else
	{
		SQL_PARAM_INFO_LIST sql_param_info_list = sql_cache_it->second;
		SQL_PARAM_INFO_IT sql_para_info_it = sql_param_info_list.begin();
		int count  = 0;
		for (; sql_para_info_it != sql_param_info_list.end(); sql_para_info_it++)
		{
			result.replace(sql_para_info_it->pos - count   , sql_para_info_it->param.length() + 3, sql_param[sql_para_info_it->param]);
			count += sql_para_info_it->param.length()  - sql_param[sql_para_info_it->param].length() + 3 ;
		}

	}
}

 
