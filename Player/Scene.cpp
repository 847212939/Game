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
bool Scene::CreatePlayr(int index, const TCPSocketInfo* pSockInfo, std::string& userId)
{
	m_PlayerCenter.CreatePlayr(index, pSockInfo, userId);
	return true;
}

// ��ʼ���ص�����
void Scene::InitCallBackFun()
{

}

// ��ȡ���Ԥ����
PlayerPreproces* Scene::GetPlayerPreproces()
{
	return m_pPlayerPreproces;
}