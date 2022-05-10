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
		COUT_LOG(LOG_CERROR, "�������ݿ�ʧ��:%s", excep.errorInfo.c_str());
	}
}

RegisteredAccount::~RegisteredAccount()
{

}

// ������Ϣ
void RegisteredAccount::HandelMessage(PlayerInfo* pInfo)
{
	// websocket������
	if (pInfo->uSrverType == SocketType::SOCKET_TYPE_WEBSOCKET)
	{

	}
	else // ����������
	{

	}
}

// �ַ���Ϣ
void RegisteredAccount::DispatchMessage()
{
	m_PlayerCenter.DispatchMessage();
}

// ������ɫ
bool RegisteredAccount::CreatePlayr()
{
	return m_PlayerCenter.CreatePlayr();
}