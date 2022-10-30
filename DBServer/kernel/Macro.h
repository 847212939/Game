#pragma once

#define G_Util				Util::Instance()
#define G_LuaMgr			CLuaMgr::Instance()
#define G_LogMgr			CGameLogManage::Instance()
#define G_CfgMgr			G_LuaMgr->GetConfigMgr()
#define G_BaseCfgMgr		G_CfgMgr->GetCBaseCfgMgr()

#define G_NetClient			G_Util->GetTCPClient()
#define G_PlayerPrepClient	G_NetClient->GetPlayerPrepClient()

#if defined(_WIN32)
#define SOCKFD SOCKET
#elif defined(_WIN64)
#elif defined(__linux__)
#define SOCKFD int
#elif defined(__unix__)
#elif defined(__ANDROID__)
#elif defined(__APPLE__)
#endif

#define	MIN_WEBSOCKET_HEAD_SIZE			2	//websocket���ݰ���С��ͷ��ʵ�ʰ�ͷ��С2-10
#define	MAX_WEBSOCKET_HEAD_SIZE			10	//websocket���ݰ���С��ͷ��ʵ�ʰ�ͷ��С2-10
#define MAGIC_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11" // websocket key
#define WS_FRAGMENT_FIN (1 << 7)

#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"libssl.lib")

//#define CA_CERT_FILE "server/ca.crt"
//#define SERVER_CERT_FILE "server/server.crt"
//#define SERVER_KEY_FILE "server/server.key"

#define CA_CERT_FILE "../Res/openssl/bin/mycert.crt"
#define SERVER_CERT_FILE "../Res/openssl/bin/mycert.crt"
#define SERVER_KEY_FILE "../Res/openssl/bin/mycert_no_passwd.key"

// Get Set mem
#define GetSetMem(type,Name)\
private:\
	type m_##Name;\
public:\
	type Get##Name() const\
	{\
		return this->m_##Name;\
	}\
public:\
	void Set##Name(type _arg)\
	{\
		this->m_##Name=_arg;\
	}\
private:

// ���ͷ��������÷�ʽ
#define GetClass(type,Name)\
private:\
	type m_##Name;\
public:\
	type& Get##Name()\
	{\
		return this->m_##Name;\
	}\
private:

// �жϴ�С����
#define Min_(x,y) ((x)>(y)?(y):(x))
#define Max_(x,y) ((x)>(y)?(x):(y))

// ��������ά��
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

// ��ȫɾ��ָ��
#define SafeDelete(pData)\
{\
	try\
	{\
		delete pData;\
	}\
	catch (...)\
	{\
		CLog::Write(G_LogMgr->GetErrorLog().c_str(), CERR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR");\
	}\
	pData = nullptr;\
} 

// ��ȫɾ��ָ��
#define  SafeDeleteArray(pData)\
{\
	if (pData)\
	{\
		try\
		{\
			delete[] pData;\
		}\
		catch (...)\
		{\
			CLog::Write(G_LogMgr->GetErrorLog().c_str(), CERR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR");\
		}\
	}\
}

// ��ȫɾ��ָ��
#define SAFE_DELETE(pData)\
{\
	if(pData)\
	{\
		try\
		{\
			delete pData;\
		}\
		catch(...)\
		{\
			CLog::Write(G_LogMgr->GetErrorLog().c_str(), CERR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR");\
		}\
		pData = nullptr;\
	}\
}

// ��־��ӡ
#define Log(logtype, ...)\
if (logtype == ERR)\
{\
	CLog::Write(G_LogMgr->GetErrorLog().c_str(), ERR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (logtype == INF)\
{\
	CLog::Write(G_LogMgr->GetErrorLog().c_str(), INF, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (logtype == WAR)\
{\
	CLog::Write(G_LogMgr->GetErrorLog().c_str(), WAR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (logtype == CERR)\
{\
	CLog::Write(G_LogMgr->GetErrorLog().c_str(), CERR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (logtype == CINF)\
{\
	CLog::Write(G_LogMgr->GetErrorLog().c_str(), CINF, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (logtype == SYS)\
{\
	CLog::Write(G_LogMgr->GetErrorLog().c_str(), SYS, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else\
{\
	CLog::Write(G_LogMgr->GetErrorLog().c_str(), CERR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
}

// ע�����ݿ�ص�
#define RegisterMysql(pobj, name, sql)\
if (!pobj)\
{\
	CLog::Write(G_LogMgr->GetErrorLog().c_str(), CERR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->AddMysqlCallback(sql, std::move(std::bind(&name, this, std::placeholders::_1)));\
}

// ע������Э��
#define RegisterNetwk(pobj, name, cmd)\
if (!pobj)\
{\
	CLog::Write(G_LogMgr->GetErrorLog().c_str(), CERR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->AddNetCallback(cmd, std::move(std::bind(&name, this, std::placeholders::_1)));\
}
