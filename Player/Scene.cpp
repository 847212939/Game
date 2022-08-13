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

bool Scene::EnterScene(Animal* animal, int sceneid, Transform transform)
{
	if (!animal)
	{
		COUT_LOG(LOG_CERROR, "animal = null");
		return false;
	}

	// 更新
	UpdateScene(animal, sceneid, transform);
	return true;
}

void Scene::UpdateScene(Animal* animal, int sceneid, Transform transform)
{
	DelSceneAnimalMap(animal->GetSceneid(), animal);
	AddSceneAnimalMap(sceneid, animal);
	animal->SetSceneid(sceneid);
	animal->Settransform(transform);
}

void Scene::AddSceneAnimalMap(int sceneid, Animal* animal)
{
	SceneAnimalMap::iterator it = m_SceneAnimalMap.find(sceneid);
	if (it == m_SceneAnimalMap.end())
	{
		std::map<uint64_t, Animal*> tmpMap;
		tmpMap.insert({ animal->GetID(), animal });
		m_SceneAnimalMap.insert({ sceneid, tmpMap });
	}
	else
	{
		it->second.insert({ animal->GetID(), animal });
	}
}

void Scene::DelSceneAnimalMap(int sceneid, Animal* animal)
{
	SceneAnimalMap::iterator it = m_SceneAnimalMap.find(sceneid);
	if (it == m_SceneAnimalMap.end())
	{
		return;
	}

	std::map<uint64_t, Animal*>::iterator pos = it->second.find(animal->GetID());
	if (pos == it->second.end())
	{
		return;
	}

	it->second.erase(pos);
}

