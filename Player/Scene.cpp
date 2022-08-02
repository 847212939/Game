#include "../Game/stdafx.h"

Scene::Scene()
{
	
}

Scene::~Scene()
{

}

void Scene::Init()
{
	DPlayerCenterClient->Init();
}

// 分发消息
void Scene::MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo)
{
	if (MsgCmd::MsgCmd_Scene == cmd)
	{
		if (!playerInfo)
		{
			COUT_LOG(LOG_CERROR, "playerInfo = null cmd = %d", (int)cmd);
			return;
		}
		SocketReadLine* pMsg = playerInfo->pMsg;
		if (!pMsg)
		{
			COUT_LOG(LOG_CERROR, "pMsg = null cmd = %d", (int)cmd);
			return;
		}
		DPlayerPrepClient->CallBackFun((MsgCmd)pMsg->netMessageHead.uAssistantID, playerInfo);
	}
	else
	{
		DPlayerCenterClient->MessageDispatch(cmd, playerInfo);
	}
}
