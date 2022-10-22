#pragma once

#define DUtil		Util::Instance()
#define LuaMgr		CLuaMgr::Instance()
#define LogMgr		CGameLogManage::Instance()
#define CfgMgr		LuaMgr->GetConfigMgr()
#define BaseCfgMgr	CfgMgr->GetCBaseCfgMgr()

#define DTCPC		DUtil->GetTCPClient()
#define DPPC		DTCPC->GetPlayerPrepClient()
#define DSC			DPPC->GetSceneClient()
#define DPCC		DSC->GetPlayerCenterClient()

#if defined(_WIN32)
#define SOCKFD SOCKET
#elif defined(_WIN64)
#elif defined(__linux__)
#define SOCKFD int
#elif defined(__unix__)
#elif defined(__ANDROID__)
#elif defined(__APPLE__)
#endif

#define	MIN_WEBSOCKET_HEAD_SIZE			2	//websocket数据包最小包头，实际包头大小2-10
#define	MAX_WEBSOCKET_HEAD_SIZE			10	//websocket数据包最小包头，实际包头大小2-10
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

// 类型返回以引用方式
#define GetClass(type,Name)\
private:\
	type m_##Name;\
public:\
	type& Get##Name()\
	{\
		return this->m_##Name;\
	}\
private:

// 判断大小函数
#define Min_(x,y) ((x)>(y)?(y):(x))
#define Max_(x,y) ((x)>(y)?(x):(y))

// 计算数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

// 安全删除指针
#define SafeDelete(pData)\
{\
	try\
	{\
		delete pData;\
	}\
	catch (...)\
	{\
		CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR");\
	}\
	pData = nullptr;\
} 

// 安全删除指针
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
			CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR");\
		}\
	}\
}

// 安全删除指针
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
			CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR");\
		}\
		pData = nullptr;\
	}\
}

// 日志打印
#define COUT_LOG(ERRTYPE, ...)\
if (ERRTYPE == LOG_ERROR)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_ERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (ERRTYPE == LOG_INFO)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_INFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (ERRTYPE == LOG_WARN)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_WARN, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (ERRTYPE == LOG_CERROR)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (ERRTYPE == LOG_CINFO)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CINFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (ERRTYPE == LOG_ERROR_SYS)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_ERROR_SYS, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
}

// 注册数据库回调
#define RegisterMysql(pobj, name, sql)\
if (!pobj)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "注册消息失败 请检查写法");\
}\
else\
{\
	pobj->AddMysqlCallback(sql, std::move(std::bind(&name, this, std::placeholders::_1)));\
}

// 注册网络协议
#define RegisterNetwk(pobj, name, cmd)\
if (!pobj)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "注册消息失败 请检查写法");\
}\
else\
{\
	pobj->AddNetCallback(cmd, std::move(std::bind(&name, this, std::placeholders::_1)));\
}
