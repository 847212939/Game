#include "../stdafx.h"

PlayerPrepClient::PlayerPrepClient() : 
	PlayerPrep(), 
	m_SceneClient(this),
	m_LoginSys(this),
	m_CrossSys(this)
{
}
PlayerPrepClient::~PlayerPrepClient()
{
}

SceneClient* PlayerPrepClient::GetSceneClient()
{
	return &m_SceneClient;
}

void PlayerPrepClient::SendOperateResults(SocketReadLine* pMsg, bool results, uint64_t userid)
{
	if (!pMsg) return;

	Netmsg msg;
	msg << (int)results;
	G_NetClient->SendMsg(pMsg->uIndex, msg.os_str().c_str(), msg.os_size(),
		MsgCmd(pMsg->netMessageHead.uMainID), 
		pMsg->netMessageHead.uAssistantID, 0, 
		pMsg->pBufferevent, 
		pMsg->netMessageHead.uIdentification, 
		userid);
}