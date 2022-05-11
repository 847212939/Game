#include "../Game/stdafx.h"

RegisteredAccount::RegisteredAccount(TCPClient* pTCPClient) : m_TCPClient(pTCPClient)
{
	InitDB();
	Run();
}

RegisteredAccount::~RegisteredAccount()
{

}

// 启动数据库
bool RegisteredAccount::InitDB()
{
	// 链接数据库
	m_CMysqlHelper.init("127.0.0.1", "root", "", "game", "", 3366);
	try
	{
		m_CMysqlHelper.connect();
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "连接数据库失败:%s", excep.errorInfo.c_str());
		return false;
	}

	COUT_LOG(LOG_CINFO, "DB connect successed...");
	return true;
}

bool RegisteredAccount::Run()
{
	return true;
}

// 处理消息
void RegisteredAccount::HandelMessage(PlayerInfo* pInfo)
{
	// websocket服务器
	if (pInfo->uSrverType == SocketType::SOCKET_TYPE_WEBSOCKET)
	{

	}
	else // 其他服务器
	{

	}
}

// 分发消息
void RegisteredAccount::DispatchMessage()
{
	m_PlayerCenter.DispatchMessage();
}

// 创建角色
bool RegisteredAccount::CreatePlayr()
{
	return m_PlayerCenter.CreatePlayr();
}

// 获取通知条件变量
ConditionVariable& RegisteredAccount::GetConditionVariable()
{
	return m_cond;
}