#include "../Game/stdafx.h"

RegisteredAccount::RegisteredAccount(TCPClient* pTCPClient) : m_TCPClient(pTCPClient)
{
	InitDB();
	Run();
}

RegisteredAccount::~RegisteredAccount()
{

}

// �������ݿ�
bool RegisteredAccount::InitDB()
{
	// �������ݿ�
	m_CMysqlHelper.init("127.0.0.1", "root", "", "game", "", 3366);
	try
	{
		m_CMysqlHelper.connect();
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "�������ݿ�ʧ��:%s", excep.errorInfo.c_str());
		return false;
	}

	COUT_LOG(LOG_CINFO, "DB connect successed...");
	return true;
}

bool RegisteredAccount::Run()
{
	return true;
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

// ��ȡ֪ͨ��������
ConditionVariable& RegisteredAccount::GetConditionVariable()
{
	return m_cond;
}