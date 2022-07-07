#include "../Game/stdafx.h"

Scene::Scene() :
	m_SubPlayerPreproces(nullptr)
{
	
}

Scene::~Scene()
{

}

void Scene::Init()
{
	m_SubPlayerCenter.SetSubScene(dynamic_cast<SubScene*>(this));
	m_SubPlayerCenter.Init();
}

// �ַ���Ϣ
void Scene::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "Scene Dispatch message player preproces = null cmd = %d", cmd);
		return;
	}
	switch (cmd)
	{
	case MsgCmd::MsgCmd_Scene:
		// ��������Ϣ.. �������ڸ�����Ϣ
		m_SubPlayerPreproces->CallBackFun(cmd, pPlayerInfo);
		break;
	default:
		m_SubPlayerCenter.DispatchMessage(cmd, pPlayerInfo);
		break;
	}
}

void Scene::SetSubPlayerPreproces(SubPlayerPreproces* pSubPlayerPreproces)
{
	m_SubPlayerPreproces = pSubPlayerPreproces;
}

// ��ȡ�������
SubPlayerCenter& Scene::GetPlayerCenter()
{ 
	return m_SubPlayerCenter; 
}

// ��ȡ���Ԥ����
SubPlayerPreproces* Scene::GetPlayerPreproces()
{ 
	return m_SubPlayerPreproces;
}
