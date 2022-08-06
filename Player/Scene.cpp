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

// ��ȡ�������
PlayerCenterClient& Scene::GetPlayerCenterClient()
{ 
	return m_PlayerCenterClient; 
}

// ��ȡ���Ԥ����
PlayerPrepClient* Scene::GetPlayerPrepClient()
{ 
	return m_PlayerPrepClient;
}
