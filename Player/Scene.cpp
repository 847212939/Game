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
	m_SubPlayerCenter.SetSubScene(dynamic_cast<SubScene*>(this));
	m_SubPlayerCenter.Init();
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
		m_SubPlayerCenter.DispatchMessage(cmd, pPlayerInfo);
	}
}

void Scene::SetSubPlayerPreproces(SubPlayerPreproces* pSubPlayerPreproces)
{
	m_SubPlayerPreproces = pSubPlayerPreproces;
}

// 获取玩家中心
SubPlayerCenter& Scene::GetPlayerCenter()
{ 
	return m_SubPlayerCenter; 
}

// 获取玩家预处理
SubPlayerPreproces* Scene::GetPlayerPreproces()
{ 
	return m_SubPlayerPreproces;
}
