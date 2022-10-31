#include "pch.h"

CMysqlHelper::CMysqlHelper() : m_bConnected(false)
{
	m_pstMql = mysql_init(nullptr);
}

CMysqlHelper::CMysqlHelper(const std::string& sHost, const std::string& sUser, const std::string& sPasswd, const std::string& sDatabase, const std::string& sCharSet, int port, int iFlag)
	: m_bConnected(false)
{
	init(sHost, sUser, sPasswd, sDatabase, sCharSet, port, iFlag);

	m_pstMql = mysql_init(nullptr);
}

CMysqlHelper::CMysqlHelper(const DBConf& tcDBConf)
	:m_bConnected(false)
{
	m_dbConf = tcDBConf;

	m_pstMql = mysql_init(nullptr);
}

CMysqlHelper::~CMysqlHelper()
{
	if (m_pstMql != nullptr)
	{
		mysql_close(m_pstMql);
		m_pstMql = nullptr;
	}
}

void CMysqlHelper::init(const std::string& sHost, const std::string& sUser, const std::string& sPasswd, const std::string& sDatabase, const std::string& sCharSet, int port, int iFlag)
{
	m_dbConf.host = sHost;
	m_dbConf.user = sUser;
	m_dbConf.password = sPasswd;
	m_dbConf.database = sDatabase;
	m_dbConf.charset = sCharSet;
	m_dbConf.port = port;
	m_dbConf.flag = iFlag;
}

void CMysqlHelper::init(const DBConf& tcDBConf)
{
	m_dbConf = tcDBConf;
}

void CMysqlHelper::connect()
{
	disconnect();

	if (m_pstMql == nullptr)
	{
		m_pstMql = mysql_init(nullptr);
	}

	//建立连接后, 自动调用设置字符集语句
	if (!m_dbConf.charset.empty())
	{
		if (mysql_options(m_pstMql, MYSQL_SET_CHARSET_NAME, m_dbConf.charset.c_str()))
		{
			throw MysqlHelper_Exception(std::string("CMysqlHelper::connect: mysql_options MYSQL_SET_CHARSET_NAME ") + m_dbConf.charset + ":" + std::string(mysql_error(m_pstMql)));
		}
	}

	if (mysql_real_connect(m_pstMql, m_dbConf.host.c_str(), m_dbConf.user.c_str(), m_dbConf.password.c_str(), m_dbConf.database.c_str(), m_dbConf.port, nullptr, m_dbConf.flag) == nullptr)
	{
		throw MysqlHelper_Exception("[CMysqlHelper::connect]: mysql_real_connect: " + std::string(mysql_error(m_pstMql)));
	}

	m_bConnected = true;
}

void CMysqlHelper::disconnect()
{
	if (m_pstMql != nullptr)
	{
		mysql_close(m_pstMql);
		m_pstMql = mysql_init(nullptr);
	}

	m_bConnected = false;
}

std::string CMysqlHelper::escapeString(const std::string& sFrom)
{
	if (!m_bConnected)
	{
		connect();
	}

	std::string sTo;
	std::string::size_type iLen = sFrom.length() * 2 + 1;
	char* pTo = (char*)malloc(iLen);
	if (pTo)
	{
		memset(pTo, 0x00, iLen);

		mysql_real_escape_string(m_pstMql, pTo, sFrom.c_str(), static_cast<unsigned long>(sFrom.length()));

		sTo = pTo;
	}

	free(pTo);

	return sTo;
}

MYSQL* CMysqlHelper::getMysql(void)
{
	return m_pstMql;
}

std::string CMysqlHelper::buildInsertSQL(const std::string& sTableName, const MapRecordData& mpColumns)
{
	std::ostringstream sColumnNames;
	std::ostringstream sColumnValues;

	std::map<std::string, std::pair<FT, std::string> >::const_iterator itEnd = mpColumns.end();

	for (std::map<std::string, std::pair<FT, std::string> >::const_iterator it = mpColumns.begin(); it != itEnd; ++it)
	{
		if (it == mpColumns.begin())
		{
			sColumnNames << "`" << it->first << "`";
			if (it->second.first == FT::DB_INT)
			{
				sColumnValues << it->second.second;
			}
			else
			{
				sColumnValues << "'" << escapeString(it->second.second) << "'";
			}
		}
		else
		{
			sColumnNames << ",`" << it->first << "`";
			if (it->second.first == FT::DB_INT)
			{
				sColumnValues << "," + it->second.second;
			}
			else
			{
				sColumnValues << ",'" + escapeString(it->second.second) << "'";
			}
		}
	}

	std::ostringstream msg;
	msg << "insert into " << sTableName << " (" << sColumnNames.str() << ") values (" << sColumnValues.str() << ")";
	return msg.str();
}

std::string CMysqlHelper::buildReplaceSQL(const std::string& sTableName, const MapRecordData& mpColumns)
{
	std::ostringstream sColumnNames;
	std::ostringstream sColumnValues;

	std::map<std::string, std::pair<FT, std::string> >::const_iterator itEnd = mpColumns.end();
	for (std::map<std::string, std::pair<FT, std::string> >::const_iterator it = mpColumns.begin(); it != itEnd; ++it)
	{
		if (it == mpColumns.begin())
		{
			sColumnNames << "`" << it->first << "`";
			if (it->second.first == FT::DB_INT)
			{
				sColumnValues << it->second.second;
			}
			else
			{
				sColumnValues << "'" << escapeString(it->second.second) << "'";
			}
		}
		else
		{
			sColumnNames << ",`" << it->first << "`";
			if (it->second.first == FT::DB_INT)
			{
				sColumnValues << "," + it->second.second;
			}
			else
			{
				sColumnValues << ",'" << escapeString(it->second.second) << "'";
			}
		}
	}

	std::ostringstream msg;
	msg << "replace into " << sTableName << " (" << sColumnNames.str() << ") values (" << sColumnValues.str() << ")";
	return msg.str();
}

std::string CMysqlHelper::buildUpdateSQL(const std::string& sTableName, const MapRecordData& mpColumns, const std::string& sCondition)
{
	std::ostringstream sColumnNameValueSet;

	std::map<std::string, std::pair<FT, std::string> >::const_iterator itEnd = mpColumns.end();

	for (std::map<std::string, std::pair<FT, std::string> >::const_iterator it = mpColumns.begin(); it != itEnd; ++it)
	{
		if (it == mpColumns.begin())
		{
			sColumnNameValueSet << "`" << it->first << "`";
		}
		else
		{
			sColumnNameValueSet << ",`" << it->first << "`";
		}

		if (it->second.first == FT::DB_INT)
		{
			sColumnNameValueSet << "= " << it->second.second;
		}
		else
		{
			sColumnNameValueSet << "= '" << escapeString(it->second.second) << "'";
		}
	}

	std::ostringstream msg;
	msg << "update " << sTableName << " set " << sColumnNameValueSet.str() << " " << sCondition;

	return msg.str();
}

std::string CMysqlHelper::getVariables(const std::string& sName)
{
	std::string sql = "SHOW VARIABLES LIKE '" + sName + "'";

	MysqlData data;
	queryRecord(sql, data);

	if (data.size() == 0)
	{
		return "";
	}

	if (sName == data[0]["Variable_name"])
	{
		return data[0]["Value"];
	}

	return "";
}

void CMysqlHelper::execute(const std::string& sSql)
{
	// 没有连上, 连接数据库
	if (!m_bConnected)
	{
		connect();
	}

	int iRet = mysql_real_query(m_pstMql, sSql.c_str(), static_cast<unsigned long>(sSql.length()));
	if (iRet != 0)
	{
		// 自动重新连接
		int iErrno = mysql_errno(m_pstMql);
		if (iErrno == 2013 || iErrno == 2006)
		{
			connect();
			iRet = mysql_real_query(m_pstMql, sSql.c_str(), static_cast<unsigned long>(sSql.length()));
		}
	}

	if (iRet != 0)
	{
		throw MysqlHelper_Exception("[CMysqlHelper::execute]: mysql_query: [ " + sSql + " ] :" + std::string(mysql_error(m_pstMql)));
	}
}

bool CMysqlHelper::queryRecord(const std::string& sSql, MysqlData& data, bool bCharacterSet/* = false*/)
{
	/**
	没有连上, 连接数据库
	*/
	if (!m_bConnected)
	{
		connect();
	}

	if (bCharacterSet)
	{
		MYSQL_RES* pRes;
		mysql_query(m_pstMql, "SET NAMES UTF8"); //设置编码格式 Windows： SET NAMES GBK，Linux ： SET NAMES UTF8
		do
		{
			pRes = mysql_use_result(m_pstMql);
			mysql_free_result(pRes);
		} while (!mysql_next_result(m_pstMql));
	}

	int iRet = mysql_real_query(m_pstMql, sSql.c_str(), static_cast<unsigned long>(sSql.length()));
	if (iRet != 0)
	{
		/**
		自动重新连接
		*/
		int iErrno = mysql_errno(m_pstMql);
		if (iErrno == 2013 || iErrno == 2006)
		{
			connect();
			iRet = mysql_real_query(m_pstMql, sSql.c_str(), static_cast<unsigned long>(sSql.length()));
		}
	}

	if (iRet != 0)
	{
		throw MysqlHelper_Exception("[CMysqlHelper::execute]: mysql_query: [ " + sSql + " ] :" + std::string(mysql_error(m_pstMql)));
	}

	MYSQL_RES* pstRes = mysql_store_result(m_pstMql);

	if (pstRes == nullptr)
	{
		throw MysqlHelper_Exception("[CMysqlHelper::queryRecord]: mysql_store_result: " + sSql + " : " + std::string(mysql_error(m_pstMql)));
	}

	std::vector<std::string> vtFields;
	MYSQL_FIELD* field;
	while ((field = mysql_fetch_field(pstRes)))
	{
		vtFields.push_back(field->name);
	}

	std::map<std::string, std::string> mpRow;
	MYSQL_ROW stRow;

	while ((stRow = mysql_fetch_row(pstRes)) != (MYSQL_ROW)nullptr)
	{
		mpRow.clear();
		unsigned long* lengths = mysql_fetch_lengths(pstRes);
		for (size_t i = 0; i < vtFields.size(); i++)
		{
			if (stRow[i])
			{
				mpRow[vtFields[i]] = std::string(stRow[i], lengths[i]);
			}
			else
			{
				mpRow[vtFields[i]] = "";
			}
		}

		data.data().push_back(mpRow);
	}

	mysql_free_result(pstRes);

	return true;
}

void CMysqlHelper::sqlExec(const char* sql, bool bSetGBK/* = false*/)
{
	/**
	没有连上, 连接数据库
	*/
	if (!m_bConnected)
	{
		connect();
	}

	if (bSetGBK)
	{
		MYSQL_RES* pRes;
		mysql_query(m_pstMql, "SET NAMES GBK"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
		do
		{
			pRes = mysql_use_result(m_pstMql);
			mysql_free_result(pRes);
		} while (!mysql_next_result(m_pstMql));
	}

	int iRet = mysql_real_query(m_pstMql, sql, static_cast<unsigned long>(strlen(sql)));
	if (iRet != 0)
	{
		/**
		自动重新连接
		*/
		int iErrno = mysql_errno(m_pstMql);
		if (iErrno == 2013 || iErrno == 2006)
		{
			connect();
			iRet = mysql_real_query(m_pstMql, sql, static_cast<unsigned long>(strlen(sql)));
		}
	}

	if (iRet != 0)
	{
		throw MysqlHelper_Exception("[CMysqlHelper::execute]: mysql_query: [ " + std::string(sql) + " ] :" + std::string(mysql_error(m_pstMql)));
	}

	MYSQL_RES* pRes = nullptr;
	do
	{
		pRes = mysql_use_result(m_pstMql);
		if (pRes == nullptr)
		{
			return;
		}
		mysql_free_result(pRes);
	} while (!mysql_next_result(m_pstMql));
}

size_t CMysqlHelper::updateRecord(const std::string& sTableName, const MapRecordData& mpColumns, const std::string& sCondition)
{
	std::string sSql = buildUpdateSQL(sTableName, mpColumns, sCondition);
	execute(sSql);

	return (size_t)mysql_affected_rows(m_pstMql);
}

size_t CMysqlHelper::insertRecord(const std::string& sTableName, const MapRecordData& mpColumns)
{
	std::string sSql = buildInsertSQL(sTableName, mpColumns);
	execute(sSql);

	return (size_t)mysql_affected_rows(m_pstMql);
}

size_t CMysqlHelper::replaceRecord(const std::string& sTableName, const MapRecordData& mpColumns)
{
	std::string sSql = buildReplaceSQL(sTableName, mpColumns);
	execute(sSql);

	return (size_t)mysql_affected_rows(m_pstMql);
}

size_t CMysqlHelper::deleteRecord(const std::string& sTableName, const std::string& sCondition)
{
	std::ostringstream sSql;
	sSql << "delete from " << sTableName << " " << sCondition;

	execute(sSql.str());

	return (size_t)mysql_affected_rows(m_pstMql);
}

size_t CMysqlHelper::getRecordCount(const std::string& sTableName, const std::string& sCondition)
{
	std::ostringstream sSql;
	sSql << "select count(*) as num from " << sTableName << " " << sCondition;

	MysqlData data;
	queryRecord(sSql.str(), data);

	long n = atol(data[0]["num"].c_str());

	return n;

}

size_t CMysqlHelper::getSqlCount(const std::string& sCondition)
{
	std::ostringstream sSql;
	sSql << "select count(*) as num " << sCondition;

	MysqlData data;
	queryRecord(sSql.str(), data);

	long n = atol(data[0]["num"].c_str());

	return n;
}

int CMysqlHelper::getMaxValue(const std::string& sTableName, const std::string& sFieldName, const std::string& sCondition)
{
	std::ostringstream sSql;
	sSql << "select " << sFieldName << " as f from " << sTableName << " " << sCondition << " order by f desc limit 1";

	MysqlData data;
	queryRecord(sSql.str(), data);

	int n = 0;

	if (data.size() == 0)
	{
		n = 0;
	}
	else
	{
		n = atoi(data[0]["f"].c_str());
	}

	return n;
}

long CMysqlHelper::lastInsertID()
{
	return (long)mysql_insert_id(m_pstMql);
}

size_t CMysqlHelper::getAffectedRows()
{
	return (size_t)mysql_affected_rows(m_pstMql);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::map<std::string, std::string>>& CMysqlHelper::MysqlData::data()
{
	return _data;
}

size_t CMysqlHelper::MysqlData::size()
{
	return _data.size();
}

std::map<std::string, std::string>& CMysqlHelper::MysqlData::operator[](size_t i)
{
	return _data[i];
}