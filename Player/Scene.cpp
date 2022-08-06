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

// �ַ���Ϣ
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
		m_PlayerPrepClient->CallBackFun((MsgCmd)pMsg->netMessageHead.uAssistantID, pPlayerInfo);
	}
	else
	{
		m_PlayerCenterClient.DispatchMessage(cmd, pPlayerInfo);
	}
}

void Scene::SetSubPlayerPreproces(PlayerPrepClient* pSubPlayerPreproces)
{
	m_PlayerPrepClient = pSubPlayerPreproces;
}

// ��ȡ�������
PlayerCenterClient& Scene::GetPlayerCenter()
{ 
	return m_PlayerCenterClient; 
}

// ��ȡ���Ԥ����
PlayerPrepClient* Scene::GetPlayerPreproces()
{ 
	return m_PlayerPrepClient;
}
