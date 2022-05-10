#pragma once
#include <map>
#include <vector>
#include <string>
#include <mysql.h>

namespace DRMU
{
	/*********************
	*@brief 数据库异常类
	**********************/
	struct MysqlHelper_Exception //: public TC_Exception
	{
		MysqlHelper_Exception(const std::string& sBuffer) :errorInfo(sBuffer) {}; //: TC_Exception(sBuffer){};
		virtual ~MysqlHelper_Exception() noexcept {};

		std::string errorInfo;
	};

	/***********************
	* @brief 数据库配置接口
	***********************/
	struct DBConf
	{
		std::string host;//主机地址
		std::string user; //用户名 
		std::string password;//密码
		std::string database; //数据库
		std::string charset; //字符集
		int port;//端口
		int flag; //客户端标识

		/*****************
		* @brief 构造函数
		*****************/
		DBConf() :port(0), flag(0) {}

		/**********************************
		* @brief 读取数据库配置.
		* @param mpParam 存放数据库配置的map
		* dbhost: 主机地址
		* dbuser:用户名
		* dbpass:密码
		* dbname:数据库名称
		* dbport:端口
		**********************************/
		void loadFromMap(const std::map<std::string, std::string>& mpParam)
		{
			std::map<std::string, std::string> mpTmp = mpParam;

			host = mpTmp["dbhost"];
			user = mpTmp["dbuser"];
			password = mpTmp["dbpass"];
			database = mpTmp["dbname"];
			charset = mpTmp["charset"];
			port = atoi(mpTmp["dbport"].c_str());
			flag = 0;

			if (mpTmp["dbport"] == "")
			{
				port = 3306;
			}
		}
	};

	/**************************************************************
	* @brief:MySQL数据库操作类
	* @feature:非线程安全，通常一个线程一个CMysqlHelper对象；
	* 对于insert/update可以有更好的函数封装，保证SQL注入；
	* CMysqlHelper::DB_INT表示组装sql语句时，不加””和转义；
	* CMysqlHelper::DB_STR表示组装sql语句时，加””并转义；
	**************************************************************/
	class CMysqlHelper
	{
	public:
		/**
		* @brief 构造函数
		*/
		CMysqlHelper();
		/**
		* @brief 构造函数.
		* @param: sHost:主机IP
		* @param sUser 用户
		* @param sPasswd 密码
		* @param sDatebase 数据库
		* @param port 端口
		* @param iUnixSocket socket
		* @param iFlag 客户端标识
		*/
		CMysqlHelper(const std::string& sHost, const std::string& sUser = "", const std::string& sPasswd = "", const std::string& sDatabase = "", const std::string& sCharSet = "", int port = 0, int iFlag = 0);
		/**
		* @brief 构造函数.
		* @param tcDBConf 数据库配置
		*/
		CMysqlHelper(const DBConf& tcDBConf);
		/**
		* @brief 析构函数.
		*/
		virtual ~CMysqlHelper();
		/**
		* @brief 初始化.
		*
		* @param sHost 主机IP
		* @param sUser 用户
		* @param sPasswd 密码
		* @param sDatebase 数据库
		* @param port 端口
		* @param iUnixSocket socket
		* @param iFlag 客户端标识
		* @return 无
		*/
		void init(const std::string& sHost, const std::string& sUser = "", const std::string& sPasswd = "", const std::string& sDatabase = "", const std::string& sCharSet = "", int port = 3306, int iFlag = 0);
		/**
		* @brief 初始化.
		*
		* @param tcDBConf 数据库配置
		*/
		void init(const DBConf& tcDBConf);
		/**
		* @brief 连接数据库.
		*
		* @throws MysqlHelper_Exception
		* @return 无
		*/
		void connect();
		/**
		* @brief 断开数据库连接.
		* @return 无
		*/
		void disconnect();
		/**
		* @brief 获取数据库变量.
		* @return 数据库变量
		*/
		std::string getVariables(const std::string& sName);
		/**
		* @brief 直接获取数据库指针.
		*
		* @return MYSQL* 数据库指针
		*/
		MYSQL* getMysql();
		/**
		* @brief 字符转义.
		*
		* @param sFrom 源字符串
		* @param sTo 输出字符串
		* @return 输出字符串
		*/
		std::string escapeString(const std::string& sFrom);
		/**
		* @brief 更新或者插入数据.
		*
		* @param sSql sql语句
		* @throws MysqlHelper_Exception
		* @return
		*/
		void execute(const std::string& sSql);
		/**
		* @brief 查询出来的mysql数据
		*/
		class MysqlData
		{
		public:
			/**
			* @brief 所有数据.
			*
			* @return vector<map<string,string>>&
			*/
			std::vector<std::map<std::string, std::string> >& data();
			/**
			* 数据的记录条数
			*
			* @return size_t
			*/
			size_t size();
			/**
			* @brief 获取某一条记录.
			*
			* @param i 要获取第几条记录
			* @return MysqlRecord类型的数据，可以根据字段获取相关信息，
			*/
			std::map<std::string, std::string>& operator[](size_t i);

		protected:
			std::vector<std::map<std::string, std::string>> _data;
		};
		/**
		* @brief Query Record.
		*
		* @param sSql sql语句
		* @throws MysqlHelper_Exception
		* @return MysqlData类型的数据，可以根据字段获取相关信息
		*/
		bool queryRecord(const std::string& sSql, MysqlData& data, bool bCharacterSet = false);
		/**
		* @brief 执行sql语句
		*
		* @param sSql sql语句
		* @throws MysqlHelper_Exception
		* @return 成功返回true，失败返回false
		*/
		void sqlExec(const char* sql, bool bSetGBK = false);
		/**
		* @brief 定义字段类型，
		* DB_INT:数字类型
		* DB_STR:字符串类型
		*/
		enum FT
		{
			DB_INT,
			DB_STR,
		};
		/**
		* 数据记录
		*/
		typedef std::map<std::string, std::pair<FT, std::string> > RECORD_DATA;
		/**
		* @brief 更新记录.
		*
		* @param sTableName 表名
		* @param mpColumns 列名/值对
		* @param sCondition where子语句,例如:where A = B
		* @throws MysqlHelper_Exception
		* @return size_t 影响的行数
		*/
		size_t updateRecord(const std::string& sTableName, const RECORD_DATA& mpColumns, const std::string& sCondition);
		/**
		* @brief 插入记录.
		*
		* @param sTableName 表名
		* @param mpColumns 列名/值对
		* @throws MysqlHelper_Exception
		* @return size_t 影响的行数
		*/
		size_t insertRecord(const std::string& sTableName, const RECORD_DATA& mpColumns);
		/**
		* @brief 替换记录.
		*
		* @param sTableName 表名
		* @param mpColumns 列名/值对
		* @throws MysqlHelper_Exception
		* @return size_t 影响的行数
		*/
		size_t replaceRecord(const std::string& sTableName, const RECORD_DATA& mpColumns);
		/**
		* @brief 删除记录.
		*
		* @param sTableName 表名
		* @param sCondition where子语句,例如:where A = B
		* @throws MysqlHelper_Exception
		* @return size_t 影响的行数
		*/
		size_t deleteRecord(const std::string& sTableName, const std::string& sCondition = "");
		/**
		* @brief 获取Table查询结果的数目.
		*
		* @param sTableName 用于查询的表名
		* @param sCondition where子语句,例如:where A = B
		* @throws MysqlHelper_Exception
		* @return size_t 查询的记录数目
		*/
		size_t getRecordCount(const std::string& sTableName, const std::string& sCondition = "");
		/**
		* @brief 获取Sql返回结果集的个数.
		*
		* @param sCondition where子语句,例如:where A = B
		* @throws MysqlHelper_Exception
		* @return 查询的记录数目
		*/
		size_t getSqlCount(const std::string& sCondition = "");
		/**
		* @brief 获取字段最大值.
		*
		* @param sTableName 用于查询的表名
		* @param sFieldName 用于查询的字段
		* @param sCondition where子语句,例如:where A = B
		* @throws MysqlHelper_Exception
		* @return 查询的记录数目
		*/
		int getMaxValue(const std::string& sTableName, const std::string& sFieldName, const std::string& sCondition = "");
		/**
		* @brief 获取auto_increment最后插入得ID.
		*
		* @return ID值
		*/
		long lastInsertID();
		/**
		* @brief 构造Insert-SQL语句.
		*
		* @param sTableName 表名
		* @param mpColumns 列名/值对
		* @return string insert-SQL语句
		*/
		std::string buildInsertSQL(const std::string& sTableName, const RECORD_DATA& mpColumns);
		/**
		* @brief 构造Replace-SQL语句.
		*
		* @param sTableName 表名
		* @param mpColumns 列名/值对
		* @return string insert-SQL语句
		*/
		std::string buildReplaceSQL(const std::string& sTableName, const RECORD_DATA& mpColumns);
		/**
		* @brief 构造Update-SQL语句.
		*
		* @param sTableName 表名
		* @param mpColumns 列名/值对
		* @param sCondition where子语句
		* @return string Update-SQL语句
		*/
		std::string buildUpdateSQL(const std::string& sTableName, const RECORD_DATA& mpColumns, const std::string& sCondition);
		/**
		* @brief 获取最后执行的SQL语句.
		*
		* @return SQL语句
		*/
		std::string getLastSQL() { return m_sLastSql; }
		/**
		* @brief 获取查询影响数
		* @return int
		*/
		size_t getAffectedRows();

	protected:
		// @brief copy contructor，只申明,不定义,保证不被使用
		CMysqlHelper(const CMysqlHelper& tcMysql);
		// @brief 只申明,不定义,保证不被使用
		CMysqlHelper& operator=(const CMysqlHelper& tcMysql);

	private:
		// 数据库指针
		MYSQL* m_pstMql;
		// 数据库配置
		DBConf m_dbConf;
		// 是否已经连接
		bool m_bConnected;
		// 最后执行的sql
		std::string m_sLastSql;
	};
}
