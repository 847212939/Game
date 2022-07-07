#include "../Game/stdafx.h"

Player::Player(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId) :
	m_pTcpSockInfo(pSockInfo),
	m_userId(userId),
	m_PlayerPreproces(nullptr),
	m_index(index)
{
	m_CallBackFunMap.clear();
}

Player::~Player()
{
}

bool Player::SendData(int index, void* pData, int size, int mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification)
{
	if (!m_PlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null index = %d, userid = %s", index, m_userId.c_str());
		return false;
	}
	TCPClient* pTCPClient = m_PlayerPreproces->GetTCPClient();
	if (!pTCPClient)
	{
		COUT_LOG(LOG_CINFO, "TCP Clien is null index = %d, userid = %s", index, m_userId.c_str());
		return false;
	}
	return pTCPClient->SendData(index, pData, size, mainID, assistID, handleCode, pBufferevent, uIdentification);
}

// ��ʼ���ص�����
void Player::CallBackFunInit()
{
	AddCallBackFun(MsgCmd::MsgCmd_Move, std::move(std::bind(&Player::Move, this, std::placeholders::_1)));
}

void Player::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	CallBackFun(cmd, pPlayerInfo);
}

// �������ݿ�
void Player::LoadMysql()
{

}

bool Player::EnterScene()
{
	return true;
}

// ������Ϸ
void Player::EnterGame()
{

}

// ��ȡ���id
std::string Player::GetUserId() const
{
	return m_userId;
}

// ��ȡ�����Ϣ
const TCPSocketInfo* Player::GetTCPSocketInfo()
{
	return m_pTcpSockInfo;
}

bool Player::Move(PlayerInfo* pPlayerInfo)
{
	unsigned int uAssistantID = pPlayerInfo->m_pMsg->netMessageHead.uAssistantID;
	std::string str = (char*)pPlayerInfo->m_pData;
	CIstringstream is(str);
	unsigned int x = 0, y = 0;
	is >> x >> y;

	COUT_LOG(LOG_CINFO, "x = %u, y = %u", x, y);

	return true;
}

// ����ص�����
void Player::AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	PlayerCallBackFunMap::iterator it = m_CallBackFunMap.find(cmd);
	if (it == m_CallBackFunMap.end())
	{
		m_CallBackFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CINFO, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

// �ص�����
bool Player::CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	PlayerCallBackFunMap::iterator it = m_CallBackFunMap.find(cmd);
	if (it == m_CallBackFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", cmd);
		return false;
	}

	it->second(pPlayerInfo);
	return true;
}

// ���ݿ����
// ����һ�����ݿ�
bool Player::LoadOneSql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr)
{
	if (!m_PlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %s", m_userId.c_str());
		return false;
	}
	return m_PlayerPreproces->LoadOneSql(userId, sqlName, queryData, dataStr);
}

// ���ض������ݿ�
bool Player::LoadMulitySql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr)
{
	if (!m_PlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %s", m_userId.c_str());
		return false;
	}

	return m_PlayerPreproces->LoadMulitySql(userId, sqlName, queryData, dataStr);
}

// insert mysql
void Player::SaveInsertSQL(std::string sqlName, std::string name, std::string data, std::string keyName, std::string dataName)
{
	if (!m_PlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %s", m_userId.c_str());
		return;
	}

	m_PlayerPreproces->SaveInsertSQL(sqlName, name, data, keyName, dataName);
}

// delete mysql
void Player::SaveDeleteSQL(std::string sqlName, const std::string& sCondition)
{
	if (!m_PlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %s", m_userId.c_str());
		return;
	}

	m_PlayerPreproces->SaveDeleteSQL(sqlName, sCondition);
}

// replace mysql
void Player::SaveReplaceSQL(std::string sqlName, std::string name, std::string data, std::string keyName, std::string dataName)
{
	if (!m_PlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %s", m_userId.c_str());
		return;
	}

	m_PlayerPreproces->SaveReplaceSQL(sqlName, name, data, keyName, dataName);
}

// update mysql
void Player::SaveUpdateSQL(std::string sqlName, std::string name, std::string data, const std::string& sCondition, std::string keyName, std::string dataName)
{
	if (!m_PlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %s", m_userId.c_str());
		return;
	}

	m_PlayerPreproces->SaveUpdateSQL(sqlName, name, data, sCondition, keyName, dataName);
}