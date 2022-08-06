#include "../Game/stdafx.h"

Scene::Scene() :
	m_PlayerPrepClient(nullptr)
{
	
}

Scene::~Scene()
{

}

void Scene::Init()
{
	m_PlayerCenterClient.SetSubScene(dynamic_cast<SceneClient*>(this));
	m_PlayerCenterClient.Init();
}

// 分发消息
void Scene::DispatchMessage(MsgCmd cmd, PlayerInfo* playerInfo)
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
		m_PlayerPrepClient->CallBackFun((MsgCmd)pMsg->netMessageHead.uAssistantID, playerInfo);
	}
	else
	{
		m_PlayerCenterClient.DispatchMessage(cmd, playerInfo);
	}
}

void Scene::SetPlayerPrepClient(PlayerPrepClient* playerPrepClient)
{
	m_PlayerPrepClient = playerPrepClient;
}

// 获取玩家中心
PlayerCenterClient& Scene::GetPlayerCenterClient()
{ 
	return m_PlayerCenterClient; 
}

// 获取玩家预处理
PlayerPrepClient* Scene::GetPlayerPrepClient()
{ 
	return m_PlayerPrepClient;
}
