#pragma once
#include "def.h"

class CSQLParse
{
public:
	CSQLParse(void);
	~CSQLParse(void);
	std::string GetSQL(const std::string& SQL,SQL_PARAM & sql_param);
private:
	void SetParameter(SQL_PARAM & sql_param, HASH hash, std::string &result);
	void Parse(const std::string& sql, HASH hash);
};
