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
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "Scene Dispatch message player preproces = null cmd = %d", cmd);
		return;
	}
	switch (cmd)
	{
	case MsgCmd::MsgCmd_Scene:
		// 场景类消息.. 处理场景内各种消息
		m_SubPlayerPreproces->CallBackFun(cmd, pPlayerInfo);
		break;
	default:
		m_SubPlayerCenter.DispatchMessage(cmd, pPlayerInfo);
		break;
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
