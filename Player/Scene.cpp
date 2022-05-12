#include "../Game/stdafx.h"

Scene::Scene(PlayerPreproces* pPlayerPreproces) : 
	m_pPlayerPreproces(pPlayerPreproces),
	m_PlayerCenter(this)
{

}

Scene::~Scene()
{

}

// 分发消息
void Scene::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{

}

// 创建角色
bool Scene::CreatePlayr(PlayerInfo* pPlayerInfo)
{
	return true;
}

// 初始化回调函数
void Scene::InitCallBackFun()
{

}

// 获取玩家预处理
PlayerPreproces* Scene::GetPlayerPreproces()
{
	return m_pPlayerPreproces;
}