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

	COUT_LOG(LOG_CINFO, "�Ѿ����ڸ���Ϣ�Ļص�������� cmd = %d", cmd);
}

// �ص�����
bool Player::CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	PlayerCallBackFunMap::iterator it = m_CallBackFunMap.find(cmd);
	if (it == m_CallBackFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "û�в��ҵ���Ӧ�Ļص����� cmd = %d", cmd);
		return false;
	}

	it->second(pPlayerInfo);
	return true;
}