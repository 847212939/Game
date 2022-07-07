#include "../Game/stdafx.h"

SubPlayer::SubPlayer(const unsigned int& index, const TCPSocketInfo* pSockInfo, const std::string& userId) :
	Player(index, pSockInfo, userId)
{

}

SubPlayer::~SubPlayer()
{

}


// 初始化回调函数
void SubPlayer::CallBackFunInit()
{
	AddCallBackFun(MsgCmd::MsgCmd_Move, std::move(std::bind(&SubPlayer::Move, this, std::placeholders::_1)));
}

bool SubPlayer::Move(PlayerInfo* pPlayerInfo)
{
	unsigned int uAssistantID = pPlayerInfo->m_pMsg->netMessageHead.uAssistantID;
	std::string str = (char*)pPlayerInfo->m_pData;
	CIstringstream is(str);
	unsigned int x = 0, y = 0;
	is >> x >> y;

	COUT_LOG(LOG_CINFO, "x = %u, y = %u", x, y);

	return true;
}
