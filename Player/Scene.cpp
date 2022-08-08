#include "../Game/stdafx.h"

Scene::Scene()
{
	
}

Scene::~Scene()
{

}

void Scene::Init()
{
	DPCC->Init();
}

// 分发消息
void Scene::MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo)
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
	if (MsgCmd::MsgCmd_Scene == (MsgCmd)pMsg->netMessageHead.uIdentification)
	{
		
		DPPC->CallBackFun(cmd, playerInfo);
	}
	else
	{
		DPCC->MessageDispatch(cmd, playerInfo);
	}
}
