#include "../Game/stdafx.h"

Scene::Scene()
{
	
}

Scene::~Scene()
{

}

void Scene::Init()
{
	m_PlayerCenterClient.Init();
}

// 分发消息
void Scene::MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo)
{
	if (MsgCmd::MsgCmd_Scene == cmd)
	{
		if (!playerInfo)
		{
			COUT_LOG(LOG_CERROR, "playerInfo = null cmd = %d", (int)cmd);
			return;
		}
		SocketReadLine* pMsg = playerInfo->m_pMsg;
		if (!pMsg)
		{
			COUT_LOG(LOG_CERROR, "pMsg = null cmd = %d", (int)cmd);
			return;
		}
		DPlayerPrepClient->CallBackFun((MsgCmd)pMsg->netMessageHead.uAssistantID, playerInfo);
	}
	else
	{
		m_PlayerCenterClient.MessageDispatch(cmd, playerInfo);
	}
}

// 获取玩家中心
PlayerCenterClient& Scene::GetPlayerCenterClient()
{ 
	return m_PlayerCenterClient; 
}