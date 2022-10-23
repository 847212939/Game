#include "../stdafx.h"

PlayerPrepClient::PlayerPrepClient() : 
	PlayerPrep(), 
	m_SceneClient(this),
	m_LoginSys(this)
{
}
PlayerPrepClient::~PlayerPrepClient()
{
}

SceneClient* PlayerPrepClient::GetSceneClient()
{
	return &m_SceneClient;
}

void PlayerPrepClient::SendOperateResults(SocketReadLine* pMsg, bool results/* = true*/)
{
	if (!pMsg) return;

	Netmsg msg;
	msg << (int)results;
	DTCPC->SendMsg(pMsg->uIndex, msg.str().c_str(), msg.str().size(),
		MsgCmd(pMsg->netMessageHead.uMainID), pMsg->netMessageHead.uAssistantID, 0, pMsg->pBufferevent, 0);
}