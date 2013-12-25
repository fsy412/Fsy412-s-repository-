#include "DataBase.h"

int DataBase::_DB_Connect()
{
	if( OCIInitialize((ub4) OCI_OBJECT, (dvoid *)0,(dvoid * (*)(dvoid *, size_t)) 0,(dvoid * (*)(dvoid *, dvoid *, size_t))0,
		(void (*)(dvoid *, dvoid *)) 0 ) )
	{
		strcpy(errStr, "FAILED:OCIInitialize");
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl;  
		return OCI_ERROR;
	}

	if( OCIEnvInit( (OCIEnv **) &(DBSession->envhp), OCI_DEFAULT, (size_t) 0, (dvoid **) 0 ) )
	{
		strcpy(errStr, "FAILED:OCIEnvInit");
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl; 
		return OCI_ERROR;
	}

	if( OCIHandleAlloc( (dvoid *) (DBSession->envhp), (dvoid **) &(DBSession->errhp), OCI_HTYPE_ERROR, (size_t) 0, (dvoid **) 0 ) )
	{
		strcpy(errStr, "FAILED:OCIHandleAlloc On Error Handle");
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl; 
		return OCI_ERROR;
	}

	if( OCIHandleAlloc( (dvoid *) (DBSession->envhp), (dvoid **) &(DBSession->srvhp), OCI_HTYPE_SERVER, (size_t) 0, (dvoid **) 0 ) )
	{
		strcpy(errStr, "FAILED:OCIHandleAlloc On Srv Handle");
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl; 
		return OCI_ERROR;
	}

	if( OCIHandleAlloc( (dvoid *) (DBSession->envhp), (dvoid **) &(DBSession->svchp), OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid **) 0 ) )
	{
		strcpy(errStr, "FAILED:OCIHandleAlloc On Service Context Handle");
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl; 
		return OCI_ERROR;
	}

	if( OCIServerAttach( DBSession->srvhp, DBSession->errhp, (text *)DB_server, strlen(DB_server), 0 ) )
	{
		sprintf(errStr, "FAILED:OCIServerAttach (Can't connect to %s)", DB_server );
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl; 
		return OCI_ERROR;
	}

	if( OCIAttrSet( (dvoid *) (DBSession->svchp), OCI_HTYPE_SVCCTX, (dvoid *)(DBSession->srvhp), (ub4) 0, OCI_ATTR_SERVER, (OCIError *) (DBSession->errhp) ) )
	{
		strcpy(errStr, "FAILED:OCIAttrSet" );
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl; 
		return OCI_ERROR;
	}

	if( OCIHandleAlloc((dvoid *) (DBSession->envhp), (dvoid **)&(DBSession->authp), (ub4) OCI_HTYPE_SESSION, (size_t) 0, (dvoid **) 0 ) )
	{
		strcpy(errStr, "FAILED:OCIHandleAlloc On Authentication Handle" );
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl; 
		return OCI_ERROR;
	}

	if( OCIAttrSet((dvoid *) (DBSession->authp), (ub4) OCI_HTYPE_SESSION, (dvoid *) DB_user, (ub4) strlen(DB_user),
		(ub4) OCI_ATTR_USERNAME, DBSession->errhp ) )
	{
		strcpy(errStr, "FAILED:OCIAttrSet on USERID" );
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl; 
		return OCI_ERROR;
	}
	if( OCIAttrSet((dvoid *) (DBSession->authp), (ub4) OCI_HTYPE_SESSION, (dvoid *)DB_password, (ub4) strlen(DB_password),
		(ub4) OCI_ATTR_PASSWORD, DBSession->errhp ) )
	{
		strcpy(errStr, "FAILED:OCIAttrSet on PASSWD");
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl; 
		return OCI_ERROR;
	}

	//认证用户并建立会话
	if( _DB_CheckErr(OCISessionBegin ( DBSession->svchp,
		DBSession->errhp, DBSession->authp, OCI_CRED_RDBMS, (ub4) OCI_DEFAULT)) < 0 )
	{
		return OCI_ERROR;
	}

	if( OCIAttrSet((dvoid *) (DBSession->svchp), (ub4) OCI_HTYPE_SVCCTX, (dvoid *) (DBSession->authp), (ub4) 0,
		(ub4) OCI_ATTR_SESSION, (DBSession->errhp) ) )
	{
		strcpy(errStr, "FAILED:OCIAttrSet on session" );
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl; 
		return OCI_ERROR;
	}

	//申请语句句柄
	if( _DB_CheckErr(OCIHandleAlloc( (dvoid *) (DBSession->envhp),
		(dvoid **) &(DBSession->stmthp),    OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0))<0)
	{
		return OCI_ERROR;
	}

	if( _DB_CheckErr(OCIHandleAlloc( (dvoid *) (DBSession->envhp),
		(dvoid **) &(DBSession->selectp),    OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0)) < 0 )
	{
		return OCI_ERROR;
	}
	return OCI_SUCCESS;
}

int DataBase::_DB_PrepareSQL(string &sql)
{
	tmpParam.size = 0;
	memset(strSql, 0 ,sizeof(strSql) ); 
	sprintf(strSql, "%s", sql.c_str() );

	if( _DB_CheckErr(OCIStmtPrepare( DBSession->selectp,
		DBSession->errhp, (text *)strSql, (ub4)strlen( (char *)strSql ),
		(ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT)) < 0 )
	{
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl; 
		return DB_FAILURE;
	}

	return OCI_SUCCESS;
}
int DataBase::_DB_ExecuteSQL()
{
	//绑定变量
	OCIBind *bindHandle[50];    // 最多支持50个参数
	char fieldName[10];

	for(int i = 0; i < tmpParam.size; i++)
	{
		sprintf(fieldName, ":v%d", i+1);
		if( _DB_CheckErr(OCIBindByName(DBSession->selectp,
			&bindHandle[i], DBSession->errhp, (text *) fieldName,
			-1, (dvoid *)(tmpParam.vParam[i]),
			(sword) DB_MAX_FIELD_LEN, SQLT_STR, (dvoid *) 0,
			(ub2 *) 0, (ub2) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT)) < 0 )
		{
			cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl;
			return DB_FAILURE;
		}
	}

	cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] strSql = "<<strSql<<endl;
	//执行SQL
	if( _DB_CheckErr(OCIStmtExecute( DBSession->svchp, DBSession->selectp,
		DBSession->errhp, (ub4) 0, (ub4) 0, (CONST OCISnapshot *)NULL,
		(OCISnapshot *)NULL, OCI_DEFAULT) ) < 0 )
	{
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl;
		return DB_FAILURE;
	}

	//取字段个数
	int colNum = _DB_RecordSetCol();

	//结构数据绑定
	OCIDefine *defnp[100];
	// memset(tmpRec, 0, (sizeof(DB_RECORD) * colNum) + 1);
	tmpRec = new char*[50];
	for(i = 0; i < colNum; i++)
	{
		defnp[i] = (OCIDefine *) 0;
		tmpRec[i] = new char[DB_MAX_FIELD_LEN];
		//  memset(tmpRec[i], 0, sizeof(char)*DB_MAX_FIELD_LEN);
		if( _DB_CheckErr(OCIDefineByPos( DBSession->selectp,
			&defnp[i], DBSession->errhp,  i+1, (dvoid *) tmpRec[i],
			colNum*sizeof(DB_RECORD), SQLT_STR, (dvoid *)0, (ub2 *)0, (ub2 *)0, OCI_DEFAULT) ) < 0 )
		{
			cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl;
			return DB_FAILURE;
		}
	}
	// delete[] tmpRec;

	return DB_SUCCESS;
}

int DataBase::_DB_RecordSetCol()
{
	int colNum;

	if( _DB_CheckErr(OCIAttrGet(DBSession->selectp, OCI_HTYPE_STMT, &colNum, 0, OCI_ATTR_PARAM_COUNT, DBSession->errhp) ) <  0 )
	{
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl;
		return DB_FAILURE;
	}

	return colNum;
}

int DataBase::_DB_FetchData()
{
	return( _DB_CheckErr( OCIStmtFetch( DBSession->selectp, DBSession->errhp, (ub4)1, (ub4)OCI_FETCH_NEXT, (ub4) OCI_DEFAULT ) ) );
}

void DataBase::_DB_RecordSet()
{
	_DB_CleanRecordSet();
	int col = _DB_RecordSetCol();
	RecordSet.colnum = col;
	RecordSet.rownum = 0; 

	while(0 == _DB_FetchData() )
	{
		char **p = new char*[col];

		for(int k = 0; k < col; k++)
		{
			p[k] = new char[strlen(tmpRec[k]) + 1];
			memcpy(p[k], tmpRec[k], strlen(tmpRec[k]) + 1); 
		}
		RecordSet.vRecord.push_back(p);
		RecordSet.rownum++;
	}

}

int DataBase::_DB_BindParam(int paramId, int dataType, void *param)
{
	memset(tmpParam.vParam[paramId], 0, sizeof(tParam));
	cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] tmpParam.size = "<<tmpParam.size<<endl; 
	switch(dataType)
	{
	case OCI_TYPE_CHAR:
		tmpParam.paramId = paramId;
		sprintf(tmpParam.vParam[paramId], "%s", (char *)param);
		tmpParam.size = paramId + 1;
		break;
	case OCI_TYPE_LONG:
		tmpParam.paramId = paramId;
		sprintf(tmpParam.vParam[paramId], "%ld", *(long *)param);
		tmpParam.size = paramId + 1;
		break;
	}

	return DB_SUCCESS;
}

void DataBase::_DB_EndTrans(int flag)
{
	if(0 == flag)        
		OCITransCommit(DBSession->svchp, DBSession->errhp, (ub4) 0);
	else
		OCITransRollback(DBSession->svchp, DBSession->errhp, (ub4) 0);

	return;
}


void DataBase::_DB_CleanRecordSet()
{
	if( RecordSet.rownum <= 0 ){
		RecordSet.colnum = 0;
		return;
	}

	for(int k = 0; k < RecordSet.rownum; k++ )
	{
		for( int j = 0; j< RecordSet.colnum; j++ )
		{
			if( RecordSet.vRecord[k][j])
			{
				delete [] RecordSet.vRecord[k][j];
				RecordSet.vRecord[k][j]=NULL;
			}
		}

		if(RecordSet.vRecord[k])
		{
			delete [] RecordSet.vRecord[k];
			RecordSet.vRecord[k] = NULL;
		}
	}

	RecordSet.rownum = 0;
	RecordSet.colnum = 0;
	RecordSet.vRecord.clear();
}

int DataBase::_DB_ExecuteSQL_P()
{
	//绑定变量
	OCIBind *bindHandle[50];    // 最多支持50个参数
	char fieldName[10];

	for(int i = 0; i < tmpParam.size; i++)
	{
		sprintf(fieldName, ":v%d", i+1);
		if( _DB_CheckErr(OCIBindByName(DBSession->selectp,
			&bindHandle[i], DBSession->errhp, (text *) fieldName,
			-1, (dvoid *)(tmpParam.vParam[i]),
			(sword) DB_MAX_FIELD_LEN, SQLT_STR, (dvoid *) 0,
			(ub2 *) 0, (ub2) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT)) < 0 )
		{
			cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl;
			return DB_FAILURE;
		}
	}

	cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] strSql = "<<strSql<<endl;
	//执行SQL
	if( _DB_CheckErr(OCIStmtExecute( DBSession->svchp, DBSession->selectp,
		DBSession->errhp, (ub4) 1, (ub4) 0, (CONST OCISnapshot *)NULL,
		(OCISnapshot *)NULL, OCI_DEFAULT) ) < 0 )
	{
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl;
		return DB_FAILURE;
	}

	//取字段个数
	int colNum = _DB_RecordSetCol();

	//结构数据绑定
	OCIDefine *defnp[100];
	tmpRec = new char*[50];
	for(i = 0; i < colNum; i++)
	{
		defnp[i] = (OCIDefine *) 0;
		memset(tmpRec[i], 0, sizeof(char)*DB_MAX_FIELD_LEN);
		tmpRec[i] = new char[DB_MAX_FIELD_LEN];
		if( _DB_CheckErr(OCIDefineByPos( DBSession->selectp,
			&defnp[i], DBSession->errhp,  i+1, (dvoid *) (tmpRec + i),
			colNum*sizeof(DB_RECORD), SQLT_STR, (dvoid *)0, (ub2 *)0, (ub2 *)0, OCI_DEFAULT) ) < 0 )
		{
			cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] errStr = "<<errStr<<endl;
			return DB_FAILURE;
		}
	}

	return DB_SUCCESS;
}



Main函数文件：

#include "DataBase.h"

/*  测试表 ************************
create table TEST_TAB
(
ID   NUMBER(10),
NAME CHAR(10),
AGE  NUMBER(10)
)
*/



main()
{
	int flag;
	int i;

	DataBase db( "billopr", "billopr", "accpstdb" );
	flag = db._DB_Connect();
	if( flag != OCI_SUCCESS )
	{
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] db._DB_Connect() = "<<flag<<endl;
	}

	string sql;
	sql = string( "" ) + "select * from test_tab where id = 11";
	flag = db._DB_PrepareSQL(sql);

	if( flag != OCI_SUCCESS )
	{
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] db._DB_PrepareSQL() = "<<flag<<endl;
	}

	flag = db._DB_ExecuteSQL();
	if( flag != OCI_SUCCESS )
	{
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] db._DB_ExecuteSQL() = "<<flag<<endl;
	}
	db._DB_RecordSet();

	cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] RecordSet.colnum = "<<db.RecordSet.colnum<<endl;
	cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] RecordSet.rownum = "<<db.RecordSet.rownum<<endl;
	cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] RecordSet.size() = "<<db.RecordSet.size()<<endl;


	for(i = 0; i < db.RecordSet.size(); i++)
	{ 
		for(int j = 0; j < db.RecordSet.colnum; j++ )
		{
			cout<<db.RecordSet.rec(i, j)<<"/t";
		}
		cout<<endl;
	}

	sql = string( "" ) + "select * from test_tab where id = :v1 and age > :v2";
	char *num1 = "13";
	char *num2 = "0";

	flag = db._DB_PrepareSQL(sql);

	if( flag != OCI_SUCCESS )
	{
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] db._DB_PrepareSQL() = "<<flag<<endl;
	}

	flag = db._DB_BindParam(0, OCI_TYPE_CHAR, num1);
	if( flag != OCI_SUCCESS )
	{
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] db._DB_BindParam() = "<<flag<<endl;
	}
	flag = db._DB_BindParam(1, OCI_TYPE_CHAR, num2);
	if( flag != OCI_SUCCESS )
	{
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] db._DB_BindParam() = "<<flag<<endl;
	}

	flag = db._DB_ExecuteSQL();
	if( flag != OCI_SUCCESS )
	{
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] db._DB_ExecuteSQL() = "<<flag<<endl;
	}
	db._DB_RecordSet();

	cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] RecordSet.colnum = "<<db.RecordSet.colnum<<endl;
	cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] RecordSet.rownum = "<<db.RecordSet.rownum<<endl;
	cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] RecordSet.size() = "<<db.RecordSet.size()<<endl;

	// tmpRec = &(db._DB_FetchData());

	for(i = 0; i < db.RecordSet.size(); i++)
	{ 
		for(int j = 0; j < db.RecordSet.colnum; j++ )
		{
			cout<<db.RecordSet.rec(i, j)<<"/t";
		}
		cout<<endl;
	}


	sql = string( "" ) + "insert into test_tab values(:v1, :v2, :v3)";
	// char *num1 = "25";
	// char *num2 = "wwwwwwww";
	char *num3 = "30";

	flag = db._DB_PrepareSQL(sql);

	if( flag != OCI_SUCCESS )
	{
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] db._DB_PrepareSQL() = "<<flag<<endl;
	}

	flag = db._DB_BindParam(0, OCI_TYPE_CHAR, num1);
	flag = db._DB_BindParam(1, OCI_TYPE_CHAR, num2);
	flag = db._DB_BindParam(2, OCI_TYPE_CHAR, num3);

	if( flag != OCI_SUCCESS )
	{
		cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] db._DB_BindParam() = "<<flag<<endl;
	}

	flag = db._DB_ExecuteSQL_P();
	/* if( flag != OCI_SUCCESS )
	{
	cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] db._DB_ExecuteSQL() = "<<flag<<endl;
	}
	db._DB_RecordSet();

	cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] RecordSet.colnum = "<<db.RecordSet.colnum<<endl;
	cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] RecordSet.rownum = "<<db.RecordSet.rownum<<endl;
	cout<<"[ "<<__FILE__<<" ] [ "<<__LINE__<<" ] RecordSet.size() = "<<db.RecordSet.size()<<endl;

	// tmpRec = &(db._DB_FetchData());

	for(i = 0; i < db.RecordSet.size(); i++)
	{ 
	for(int j = 0; j < db.RecordSet.colnum; j++ )
	{
	cout<<db.RecordSet.rec(i, j)<<"/t";
	}
	cout<<endl;
	}*/
	db._DB_EndTrans(0);
}