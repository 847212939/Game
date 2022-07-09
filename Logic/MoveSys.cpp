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
	m_pSubPlayer->AddMysqlCallback("move", std::move(std::bind(&MoveSys::MysqlCallback, this, std::placeholders::_1)));
}

void MoveSys::MysqlCallback(std::string&& data)
{
	if (!m_pSubPlayer)
	{
		COUT_LOG(LOG_CERROR, "MoveSys sub player is null");
		return;
	}
	if (data.empty())
	{
		return;
	}

	CIstringstream is(data);
	unsigned int x = 0, y = 0;

	is >> x >> y;

	COUT_LOG(LOG_CERROR, "x = %u, y = %u", x, y);
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
	CIstringstream is((char*)pPlayerInfo->m_pData);

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

	COstringstream os;
	os << x << y;

	m_pSubPlayer->SaveReplaceSQL("move", os);

	return true;
}
