#include "../stdafx.h"

PlayerPrepClient::PlayerPrepClient() : 
	PlayerPrep()
{
	InitMysqlTable();
}
PlayerPrepClient::~PlayerPrepClient()
{
}

void PlayerPrepClient::SendOperateResults(SocketReadLine* pMsg, bool results/* = true*/)
{
	if (!pMsg) return;

	Netmsg msg;
	msg << (int)results;
	DTCPC->SendMsg(pMsg->uIndex, msg.str().c_str(), msg.str().size(),
		MsgCmd(pMsg->netMessageHead.uMainID), pMsg->netMessageHead.uAssistantID, 0, pMsg->pBufferevent, 0);
}