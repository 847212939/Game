#include "PlayerInclude.h"
#include "RegisteredAccount.h"
#include "Player.h"

using namespace PLAYER;
RegisteredAccount::RegisteredAccount()
{
	m_CMysqlHelper.init("127.0.0.1", "root", "123456", "Heart");
	try
	{
		m_CMysqlHelper.connect();
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "连接数据库失败:%s", excep.errorInfo.c_str());
	}
}

RegisteredAccount::~RegisteredAccount()
{

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