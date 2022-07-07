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
	m_PlayerCenter.DispatchMessage(cmd, pPlayerInfo);
}

// 创建角色
bool Scene::CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId)
{
	m_PlayerCenter.CreatePlayer(index, pSockInfo, userId);
	return true;
}

// 初始化回调函数
void Scene::InitCallBackFun()
{

}
