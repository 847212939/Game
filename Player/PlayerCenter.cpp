#include "../Game/stdafx.h"

PlayerCenter::PlayerCenter(Scene* pScene) : 
	m_pScene(pScene)
{

}

PlayerCenter::~PlayerCenter()
{

}

// 分发消息
void PlayerCenter::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{

}

// 初始化回调函数
void PlayerCenter::InitCallBackFun()
{

}

// 创建角色
bool PlayerCenter::CreatePlayr()
{
	return true;
}