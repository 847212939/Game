#include "../Game/stdafx.h"

PlayerPrepClient::PlayerPrepClient() : 
	PlayerPrep(), 
	m_SceneClient(this),
	m_LoginSys(this)
{
	InitMysqlTable();
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

	Netmsg os;
	os << (int)results;
	DTCPC->SendData(pMsg->uIndex, os.str().c_str(), os.str().size(),
		MsgCmd(pMsg->netMessageHead.uMainID), pMsg->netMessageHead.uAssistantID, 0, pMsg->pBufferevent, 0);
}