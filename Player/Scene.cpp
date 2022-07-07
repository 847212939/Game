#include "../Game/stdafx.h"

Scene::Scene(SubPlayerPreproces* pSubPlayerPreproces) :
	m_SubPlayerPreproces(pSubPlayerPreproces),
	m_SubPlayerCenter(dynamic_cast<SubScene*>(this))
{
	
}

Scene::~Scene()
{

}

// �ַ���Ϣ
void Scene::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message player preproces = null cmd = %d", cmd);
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
