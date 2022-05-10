#pragma once
#include <map>
#include <vector>
#include <string>
#include <mysql.h>

namespace DRMU
{
	/*********************
	*@brief ���ݿ��쳣��
	**********************/
	struct MysqlHelper_Exception //: public TC_Exception
	{
		MysqlHelper_Exception(const std::string& sBuffer) :errorInfo(sBuffer) {}; //: TC_Exception(sBuffer){};
		virtual ~MysqlHelper_Exception() noexcept {};

		std::string errorInfo;
	};

	/***********************
	* @brief ���ݿ����ýӿ�
	***********************/
	struct DBConf
	{
		std::string host;//������ַ
		std::string user; //�û��� 
		std::string password;//����
		std::string database; //���ݿ�
		std::string charset; //�ַ���
		int port;//�˿�
		int flag; //�ͻ��˱�ʶ

		/*****************
		* @brief ���캯��
		*****************/
		DBConf() :port(0), flag(0) {}

		/**********************************
		* @brief ��ȡ���ݿ�����.
		* @param mpParam ������ݿ����õ�map
		* dbhost: ������ַ
		* dbuser:�û���
		* dbpass:����
		* dbname:���ݿ�����
		* dbport:�˿�
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
	* @brief:MySQL���ݿ������
	* @feature:���̰߳�ȫ��ͨ��һ���߳�һ��CMysqlHelper����
	* ����insert/update�����и��õĺ�����װ����֤SQLע�룻
	* CMysqlHelper::DB_INT��ʾ��װsql���ʱ�����ӡ�����ת�壻
	* CMysqlHelper::DB_STR��ʾ��װsql���ʱ���ӡ�����ת�壻
	**************************************************************/
	class CMysqlHelper
	{
	public:
		/**
		* @brief ���캯��
		*/
		CMysqlHelper();
		/**
		* @brief ���캯��.
		* @param: sHost:����IP
		* @param sUser �û�
		* @param sPasswd ����
		* @param sDatebase ���ݿ�
		* @param port �˿�
		* @param iUnixSocket socket
		* @param iFlag �ͻ��˱�ʶ
		*/
		CMysqlHelper(const std::string& sHost, const std::string& sUser = "", const std::string& sPasswd = "", const std::string& sDatabase = "", const std::string& sCharSet = "", int port = 0, int iFlag = 0);
		/**
		* @brief ���캯��.
		* @param tcDBConf ���ݿ�����
		*/
		CMysqlHelper(const DBConf& tcDBConf);
		/**
		* @brief ��������.
		*/
		virtual ~CMysqlHelper();
		/**
		* @brief ��ʼ��.
		*
		* @param sHost ����IP
		* @param sUser �û�
		* @param sPasswd ����
		* @param sDatebase ���ݿ�
		* @param port �˿�
		* @param iUnixSocket socket
		* @param iFlag �ͻ��˱�ʶ
		* @return ��
		*/
		void init(const std::string& sHost, const std::string& sUser = "", const std::string& sPasswd = "", const std::string& sDatabase = "", const std::string& sCharSet = "", int port = 3306, int iFlag = 0);
		/**
		* @brief ��ʼ��.
		*
		* @param tcDBConf ���ݿ�����
		*/
		void init(const DBConf& tcDBConf);
		/**
		* @brief �������ݿ�.
		*
		* @throws MysqlHelper_Exception
		* @return ��
		*/
		void connect();
		/**
		* @brief �Ͽ����ݿ�����.
		* @return ��
		*/
		void disconnect();
		/**
		* @brief ��ȡ���ݿ����.
		* @return ���ݿ����
		*/
		std::string getVariables(const std::string& sName);
		/**
		* @brief ֱ�ӻ�ȡ���ݿ�ָ��.
		*
		* @return MYSQL* ���ݿ�ָ��
		*/
		MYSQL* getMysql();
		/**
		* @brief �ַ�ת��.
		*
		* @param sFrom Դ�ַ���
		* @param sTo ����ַ���
		* @return ����ַ���
		*/
		std::string escapeString(const std::string& sFrom);
		/**
		* @brief ���»��߲�������.
		*
		* @param sSql sql���
		* @throws MysqlHelper_Exception
		* @return
		*/
		void execute(const std::string& sSql);
		/**
		* @brief ��ѯ������mysql����
		*/
		class MysqlData
		{
		public:
			/**
			* @brief ��������.
			*
			* @return vector<map<string,string>>&
			*/
			std::vector<std::map<std::string, std::string> >& data();
			/**
			* ���ݵļ�¼����
			*
			* @return size_t
			*/
			size_t size();
			/**
			* @brief ��ȡĳһ����¼.
			*
			* @param i Ҫ��ȡ�ڼ�����¼
			* @return MysqlRecord���͵����ݣ����Ը����ֶλ�ȡ�����Ϣ��
			*/
			std::map<std::string, std::string>& operator[](size_t i);

		protected:
			std::vector<std::map<std::string, std::string>> _data;
		};
		/**
		* @brief Query Record.
		*
		* @param sSql sql���
		* @throws MysqlHelper_Exception
		* @return MysqlData���͵����ݣ����Ը����ֶλ�ȡ�����Ϣ
		*/
		bool queryRecord(const std::string& sSql, MysqlData& data, bool bCharacterSet = false);
		/**
		* @brief ִ��sql���
		*
		* @param sSql sql���
		* @throws MysqlHelper_Exception
		* @return �ɹ�����true��ʧ�ܷ���false
		*/
		void sqlExec(const char* sql, bool bSetGBK = false);
		/**
		* @brief �����ֶ����ͣ�
		* DB_INT:��������
		* DB_STR:�ַ�������
		*/
		enum FT
		{
			DB_INT,
			DB_STR,
		};
		/**
		* ���ݼ�¼
		*/
		typedef std::map<std::string, std::pair<FT, std::string> > RECORD_DATA;
		/**
		* @brief ���¼�¼.
		*
		* @param sTableName ����
		* @param mpColumns ����/ֵ��
		* @param sCondition where�����,����:where A = B
		* @throws MysqlHelper_Exception
		* @return size_t Ӱ�������
		*/
		size_t updateRecord(const std::string& sTableName, const RECORD_DATA& mpColumns, const std::string& sCondition);
		/**
		* @brief �����¼.
		*
		* @param sTableName ����
		* @param mpColumns ����/ֵ��
		* @throws MysqlHelper_Exception
		* @return size_t Ӱ�������
		*/
		size_t insertRecord(const std::string& sTableName, const RECORD_DATA& mpColumns);
		/**
		* @brief �滻��¼.
		*
		* @param sTableName ����
		* @param mpColumns ����/ֵ��
		* @throws MysqlHelper_Exception
		* @return size_t Ӱ�������
		*/
		size_t replaceRecord(const std::string& sTableName, const RECORD_DATA& mpColumns);
		/**
		* @brief ɾ����¼.
		*
		* @param sTableName ����
		* @param sCondition where�����,����:where A = B
		* @throws MysqlHelper_Exception
		* @return size_t Ӱ�������
		*/
		size_t deleteRecord(const std::string& sTableName, const std::string& sCondition = "");
		/**
		* @brief ��ȡTable��ѯ�������Ŀ.
		*
		* @param sTableName ���ڲ�ѯ�ı���
		* @param sCondition where�����,����:where A = B
		* @throws MysqlHelper_Exception
		* @return size_t ��ѯ�ļ�¼��Ŀ
		*/
		size_t getRecordCount(const std::string& sTableName, const std::string& sCondition = "");
		/**
		* @brief ��ȡSql���ؽ�����ĸ���.
		*
		* @param sCondition where�����,����:where A = B
		* @throws MysqlHelper_Exception
		* @return ��ѯ�ļ�¼��Ŀ
		*/
		size_t getSqlCount(const std::string& sCondition = "");
		/**
		* @brief ��ȡ�ֶ����ֵ.
		*
		* @param sTableName ���ڲ�ѯ�ı���
		* @param sFieldName ���ڲ�ѯ���ֶ�
		* @param sCondition where�����,����:where A = B
		* @throws MysqlHelper_Exception
		* @return ��ѯ�ļ�¼��Ŀ
		*/
		int getMaxValue(const std::string& sTableName, const std::string& sFieldName, const std::string& sCondition = "");
		/**
		* @brief ��ȡauto_increment�������ID.
		*
		* @return IDֵ
		*/
		long lastInsertID();
		/**
		* @brief ����Insert-SQL���.
		*
		* @param sTableName ����
		* @param mpColumns ����/ֵ��
		* @return string insert-SQL���
		*/
		std::string buildInsertSQL(const std::string& sTableName, const RECORD_DATA& mpColumns);
		/**
		* @brief ����Replace-SQL���.
		*
		* @param sTableName ����
		* @param mpColumns ����/ֵ��
		* @return string insert-SQL���
		*/
		std::string buildReplaceSQL(const std::string& sTableName, const RECORD_DATA& mpColumns);
		/**
		* @brief ����Update-SQL���.
		*
		* @param sTableName ����
		* @param mpColumns ����/ֵ��
		* @param sCondition where�����
		* @return string Update-SQL���
		*/
		std::string buildUpdateSQL(const std::string& sTableName, const RECORD_DATA& mpColumns, const std::string& sCondition);
		/**
		* @brief ��ȡ���ִ�е�SQL���.
		*
		* @return SQL���
		*/
		std::string getLastSQL() { return m_sLastSql; }
		/**
		* @brief ��ȡ��ѯӰ����
		* @return int
		*/
		size_t getAffectedRows();

	protected:
		// @brief copy contructor��ֻ����,������,��֤����ʹ��
		CMysqlHelper(const CMysqlHelper& tcMysql);
		// @brief ֻ����,������,��֤����ʹ��
		CMysqlHelper& operator=(const CMysqlHelper& tcMysql);

	private:
		// ���ݿ�ָ��
		MYSQL* m_pstMql;
		// ���ݿ�����
		DBConf m_dbConf;
		// �Ƿ��Ѿ�����
		bool m_bConnected;
		// ���ִ�е�sql
		std::string m_sLastSql;
	};
}
