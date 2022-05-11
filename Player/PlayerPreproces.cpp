#include "../Game/stdafx.h"

PlayerPreproces::PlayerPreproces(TCPClient* pTCPClient) : m_pTCPClient(pTCPClient)
{
	InitDB();
	Run();
}

PlayerPreproces::~PlayerPreproces()
{

}

// 启动数据库
bool PlayerPreproces::InitDB()
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

// 加载玩家账号信息
bool PlayerPreproces::LoadUserAccount()
{
	return true;
}

bool PlayerPreproces::Run()
{
	std::vector<std::thread*>& threadVec = m_pTCPClient->GetSockeThreadVec();
	threadVec.push_back(new std::thread(&PlayerPreproces::HandlerExecuteDB, this));
	return true;
}

// 数据库执行
void PlayerPreproces::HandlerExecuteDB()
{
	COUT_LOG(LOG_CINFO, "PlayerPreproces::HandlerDBSave thread begin...");
	while (m_pTCPClient->GetRuninged())
	{

	}
}

// 处理消息
void PlayerPreproces::HandlerMessage(PlayerInfo* pInfo)
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
void PlayerPreproces::DispatchMessage()
{
	m_PlayerCenter.DispatchMessage();
}

// 创建角色
bool PlayerPreproces::CreatePlayr()
{
	return m_PlayerCenter.CreatePlayr();
}

// 获取通知条件变量
ConditionVariable& PlayerPreproces::GetConditionVariable()
{
	return m_cond;
}