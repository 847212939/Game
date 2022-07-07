#include "../Game/stdafx.h"

Scene::Scene(PlayerPreproces* pPlayerPreproces) : 
	m_pPlayerPreproces(pPlayerPreproces),
	m_PlayerCenter(this)
{
	
}

Scene::~Scene()
{

}

// �ַ���Ϣ
void Scene::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	m_PlayerCenter.DispatchMessage(cmd, pPlayerInfo);
}

// ������ɫ
bool Scene::CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId)
{
	m_PlayerCenter.CreatePlayer(index, pSockInfo, userId);
	return true;
}

// ��ʼ���ص�����
void Scene::InitCallBackFun()
{

}
