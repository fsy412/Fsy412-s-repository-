#include <string>
#include <iostream>
//#include <stdlib.h>
#include <ctype.h>
#include <vector>

#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <oratypes.h>
#include <ocidfn.h>
#include <ociapr.h>
#include <ocikpr.h>
#include <oci.h>

#define ORACLE

using namespace std;
//using namespace oracle::occi;


#define DB_SUCCESS    0   /*!< 成功标志*/
#define DB_FAILURE   -1   /*!< 失败标志*/
#define DB_NO_DATA_FOUND -100   /*!< 没有数据*/

#define DB_MAX_FIELD_LEN  257   /*!< 每个字段的最大长度*/
#define DB_MAX_COL_LEN     70   /*!< 每个表的最多列数*/

/*! @brief ORACLE连接结构*/
typedef struct _db_session
{
	OCIEnv  *envhp;
	OCIServer *srvhp;
	OCIError *errhp;
	OCISvcCtx *svchp;
	OCIStmt  *stmthp;
	OCIStmt  *selectp;
	OCISession *authp;
}DB_SESSION;

/*! @brief 字段记录*/
typedef  struct _db_record
{
	char field[DB_MAX_FIELD_LEN];  /*!< 字段*/
}DB_RECORD;

//oci查询返回的记录集
typedef struct TypeRecord {
	int rownum;  //记录集的行数
	int colnum;  //记录集的列数
	vector<char **> vRecord;
	char *rec(int i,int j){if(i < rownum && j < colnum) return vRecord[i][j]; return NULL;};
	int size(){return rownum >= 0 ? rownum : 0;};
} tRecordSet;

//绑定参数结构体定义
typedef struct TypeParam{
	int paramId;
	char vParam[50][50];
	int size;
}tParam;


/*! @brief 行标识*/
typedef OCIRowid  DB_ROWID;

#define OCI_TYPE_CHAR         0     /*!< char型数据类型定义 */
#define OCI_TYPE_INT          1     /*!< int型数据类型定义 */
#define OCI_TYPE_DATE         2     /*!< time_t型数据类型定义 */
#define OCI_TYPE_FLOAT        3     /*!< fload型数据类型定义 */
#define OCI_TYPE_DOUBLE       4     /*!< double型数据类型定义 */
#define OCI_TYPE_LONG         5     /*!< long型数据类型定义 */

class DataBase
{
public:
	DataBase(const char *user, const char *password, const char *server)
	{
		strcpy( DB_user, user );
		strcpy( DB_password, password);
		strcpy( DB_server, server );
		memset( errStr, 0, sizeof(errStr) );;
		//  memset( tmpRec, 0, (sizeof(tmpRec)));

		DBSession = new DB_SESSION();
		memset( DBSession, 0, sizeof(DB_SESSION) );
	};

	short _DB_CheckErr(sword status )
	{
		char errBuf[512];
		sb4 errcode = 0;
		int i;

		switch (status)
		{
		case OCI_SUCCESS:  /*0*/
			return DB_SUCCESS;

		case OCI_SUCCESS_WITH_INFO:  /*1*/
			strcpy(errStr, "OCI_SUCCESS_WITH_INFO");
			break;
		case OCI_NEED_DATA:   /*99*/
			strcpy(errStr, "OCI_NEED_DATA");
			break;
		case OCI_NO_DATA:   /*100*/
			strcpy(errStr, "OCI_NO_DATA");
			return DB_NO_DATA_FOUND;

		case OCI_ERROR:   /*-1*/
			(void) OCIErrorGet((dvoid *)DBSession->errhp, (ub4) 1, (text *) NULL, &errcode,
				(text *)errBuf, (ub4) sizeof(errBuf), OCI_HTYPE_ERROR);
			if(1405 == errcode)
				return DB_SUCCESS;   /*列为空值时不是错误*/

			errBuf[100]='/0';
			for(i=0;i<(int)strlen(errBuf);i++)
			{
				if(errBuf[i]=='/n')
				{
					errBuf[i]='/0';
					break;
				}
			}
			strcpy(errStr, errBuf);
			break;
		case OCI_INVALID_HANDLE:   /*-2*/
			strcpy(errStr, "OCI_INVALID_HANDLE");
			break;
		case OCI_STILL_EXECUTING:   /*-3123*/
			strcpy(errStr, "OCI_STILL_EXECUTING");
			break;
		case OCI_CONTINUE:   /*-24200*/
			strcpy(errStr, "OCI_CONTINUE");
			break;
		default:
			strcpy(errStr, "DEFAULT");
		}
		return DB_FAILURE;
	};

	~DataBase()
	{
		if(NULL==DBSession)
			return;

		_DB_EndTrans(1);

		// Release SQL statement handler
		if (DBSession->stmthp != NULL)
		{
			OCIHandleFree((dvoid *)(DBSession->stmthp), OCI_HTYPE_STMT);
			DBSession->stmthp = NULL;
		}
		if (DBSession->selectp != NULL)
		{
			OCIHandleFree((dvoid *)(DBSession->selectp), OCI_HTYPE_STMT);
			DBSession->selectp = NULL;
		}
		// Delete the user session 
		if (DBSession->authp != NULL)
		{
			OCISessionEnd(DBSession->svchp, DBSession->errhp, DBSession->authp, (ub4)OCI_DEFAULT);
			DBSession->authp = NULL;
		}
		// Delete access to the data source 
		if (DBSession->errhp)
		{
			OCIServerDetach(DBSession->srvhp, DBSession->errhp, OCI_DEFAULT);
		}
		// Explicitly deallocate all handles 
		if (DBSession->srvhp != NULL)
		{
			OCIHandleFree((dvoid *)(DBSession->srvhp), OCI_HTYPE_SERVER);
			DBSession->srvhp = NULL;
		}
		if (DBSession->svchp != NULL)
		{
			OCIHandleFree((dvoid *)(DBSession->svchp), OCI_HTYPE_SVCCTX);
			DBSession->svchp = NULL;
		}
		if (DBSession->errhp != NULL)
		{
			OCIHandleFree((dvoid *)(DBSession->errhp), OCI_HTYPE_ERROR);
			DBSession->errhp = NULL;
		}
		DBSession = NULL;

		return;
	};

	//初始化DCI连接
	int _DB_Connect();
	//断开数据库
	void _DB_Disconnect();

	int _DB_PrepareSQL(string &sql);

	int _DB_ExecuteSQL();

	int _DB_ExecuteSQL_P();

	int _DB_BindParam(int paramId, int dataType, void *param);

	int _DB_RecordSetCol();

	void _DB_RecordSet();

	void _DB_CleanRecordSet();

	int _DB_FetchData();

	void _DB_EndTrans(int flag);

	tRecordSet RecordSet;

private:
	char DB_user[8];
	char DB_password[8];
	char DB_server[9];
	DB_SESSION *DBSession;
	char errStr[50];
	char strSql[1024];


	char** tmpRec;
	tParam tmpParam;

};


#endif