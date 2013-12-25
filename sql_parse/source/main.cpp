#include "def.h"
#include "SqlParse.h" 
#include "hash.h"
#include <iostream>
#include "log.hpp"

#define SQL "SELECT IPTYPE as IP_TYPE, pf_getpasswd('${User-Name}','%GHCADIALER') as User_Password,ServiceType as Service_Type,FramedProtocol as Framed_Protocol,FramedIPAddress as Framed_IP_Address, f_getipattribute_SELFDUE(BANDWIDTH,ip_attribute,state,'${NAS-Ip-Address}') as Class,FramedIPNetmask as FramedIP_Netmask,F_GetContext_LOC(nvl(contextname,'default'),nvl(bandwidth,'ABC')) as Unisphere_Virtual_Router,starttime as starttime,endtime as endtime,c.SessionTimeout as Session_Timeout,F_GETCLIENTTYPE('${User-Name}') as CLIENT_TYPE,F_GETREPLYMESSAGE_SELFDUE_OPT('${User-Name}','${NAS-Port}','${NAS-Ip-Address}','${NAS-Port-Id}') as Reply_Message from customer_table c WHERE c.loginname=f_getadslname('${User-Name}')"

SQL_CACHE  SQL_cache;
module::CLog *log;
void init_log()
{
	log = new module::CLog(".",LOG_NAME, 3,1);
}

int main()
{
 	init_log();
	std::string sql(SQL);
 
	CSQLParse sqlParse;
	SQL_PARAM sql_param;
 
	sql_param["User-Name"] ="Test";
	sql_param["NAS-Ip-Address"] = "192.168.1.1";
	sql_param["NAS-Port"] = "123";
	sql_param["NAS-Port-Id"] = "1111";
	log->PushLog(DEBUG,"test");
 
	std::cout<<sqlParse.GetSQL(sql,sql_param).c_str()<<std::endl;
	sqlParse.GetSQL(sql,sql_param);
	return 0;
}
