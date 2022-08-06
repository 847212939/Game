#include "../Game/stdafx.h"

Scene::Scene() :
	m_SubPlayerPreproces(nullptr)
{
	
}

Scene::~Scene()
{

}

void Scene::Init()
{
	m_PlayerCenterClient.SetSubScene(dynamic_cast<SubScene*>(this));
	m_PlayerCenterClient.Init();
}

// 分发消息
void Scene::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	if (MsgCmd::MsgCmd_Scene == cmd)
	{
		if (!pPlayerInfo)
		{
			COUT_LOG(LOG_CERROR, "pPlayerInfo = null cmd = %d", (int)cmd);
			return;
		}
		SocketReadLine* pMsg = pPlayerInfo->m_pMsg;
		if (!pMsg)
		{
			COUT_LOG(LOG_CERROR, "pMsg = null cmd = %d", (int)cmd);
			return;
		}
		m_SubPlayerPreproces->CallBackFun((MsgCmd)pMsg->netMessageHead.uAssistantID, pPlayerInfo);
	}
	else
	{
		m_PlayerCenterClient.DispatchMessage(cmd, pPlayerInfo);
	}
}

void Scene::SetSubPlayerPreproces(SubPlayerPreproces* pSubPlayerPreproces)
{
	m_SubPlayerPreproces = pSubPlayerPreproces;
}

// 获取玩家中心
PlayerCenterClient& Scene::GetPlayerCenter()
{ 
	return m_PlayerCenterClient; 
}

// 获取玩家预处理
SubPlayerPreproces* Scene::GetPlayerPreproces()
{ 
	return m_SubPlayerPreproces;
}
