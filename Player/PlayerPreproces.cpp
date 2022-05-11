#include "../Game/stdafx.h"

PlayerPreproces::PlayerPreproces(TCPClient* pTCPClient) : m_pTCPClient(pTCPClient)
{
	InitDB();
	Run();
}

PlayerPreproces::~PlayerPreproces()
{

}

// �������ݿ�
bool PlayerPreproces::InitDB()
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

// ��������˺���Ϣ
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

// ���ݿ�ִ��
void PlayerPreproces::HandlerExecuteDB()
{
	COUT_LOG(LOG_CINFO, "PlayerPreproces::HandlerDBSave thread begin...");
	while (m_pTCPClient->GetRuninged())
	{

	}
}

// ������Ϣ
void PlayerPreproces::HandlerMessage(PlayerInfo* pInfo)
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
void PlayerPreproces::DispatchMessage()
{
	m_PlayerCenter.DispatchMessage();
}

// ������ɫ
bool PlayerPreproces::CreatePlayr()
{
	return m_PlayerCenter.CreatePlayr();
}

// ��ȡ֪ͨ��������
ConditionVariable& PlayerPreproces::GetConditionVariable()
{
	return m_cond;
}