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

}

// ������ɫ
bool Scene::CreatePlayr(PlayerInfo* pPlayerInfo)
{
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