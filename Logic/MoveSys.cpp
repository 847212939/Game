#include "../Game/stdafx.h"

MoveSys::MoveSys(SubPlayer* pSubPlayer) :
	m_pSubPlayer(pSubPlayer)
{
	CallBackInit();
}

MoveSys::~MoveSys()
{
}

void MoveSys::CallBackInit()
{
	if (!m_pSubPlayer)
	{
		COUT_LOG(LOG_CERROR, "MoveSys sub player is null");
		return;
	}
	m_pSubPlayer->AddNetCallback(MsgCmd::MsgCmd_Move, std::move(std::bind(&MoveSys::NetworkCallback, this, std::placeholders::_1)));
	m_pSubPlayer->AddMysqlCallback(std::make_pair(m_pSubPlayer->GetUserId(), "move"), std::move(std::bind(&MoveSys::MysqlCallback, this, std::placeholders::_1)));
}

void MoveSys::MysqlCallback(std::string& data)
{
	if (!m_pSubPlayer)
	{
		COUT_LOG(LOG_CERROR, "MoveSys sub player is null");
		return;
	}
}

void MoveSys::NetworkCallback(PlayerInfo* pPlayerInfo)
{
	if (!m_pSubPlayer)
	{
		COUT_LOG(LOG_CERROR, "MoveSys sub player is null");
		return;
	}
	if (!pPlayerInfo)
	{
		COUT_LOG(LOG_CERROR, "MoveSys Player Info is null");
		return;
	}
	if (!pPlayerInfo->m_pMsg || !pPlayerInfo->m_pTcpSockInfo)
	{
		COUT_LOG(LOG_CERROR, "MoveSys player info pMsg is null or player info sock info is null");
		return;
	}
	if (!pPlayerInfo->m_pTcpSockInfo->isConnect)
	{
		COUT_LOG(LOG_CERROR, "MoveSys Network link closed");
		return;
	}

	MoveSysMsgCmd uAssistantID = (MoveSysMsgCmd)pPlayerInfo->m_pMsg->netMessageHead.uAssistantID;
	std::string str = (char*)pPlayerInfo->m_pData;
	CIstringstream is(str);

	switch (uAssistantID)
	{
	case MoveSysMsgCmd::CS_Move:
	{
		MoveCoo(is, pPlayerInfo);
		break;
	}
	default:
		break;
	}

	return;
}

bool MoveSys::MoveCoo(CIstringstream& is, PlayerInfo* pPlayerInfo)
{
	unsigned int x = 0, y = 0;
	is >> x >> y;

	COUT_LOG(LOG_CINFO, "x = %u, y = %u", x, y);

	return true;
}
