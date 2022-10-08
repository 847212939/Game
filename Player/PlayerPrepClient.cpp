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

void PlayerPrepClient::SendOperateResults(SocketReadLine* pMsg)
{
	if (!pMsg) return;

	Netmsg os;
	os << (int)true;
	DTCPC->SendData(pMsg->uIndex, os.str().c_str(), os.str().size(),
		MsgCmd(pMsg->netMessageHead.uMainID), pMsg->netMessageHead.uAssistantID, 0, pMsg->pBufferevent, 0);
}