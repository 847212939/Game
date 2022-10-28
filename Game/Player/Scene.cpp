#include "../stdafx.h"

Scene::Scene()
{
	
}
Scene::~Scene()
{

}

void Scene::Init()
{
	G_PlayerCenterClient->Init();
}

bool Scene::EnterScene(Animal* animal, int sceneid, Transform transform)
{
	if (!animal)
	{
		Log(CERR, "animal = null");
		return false;
	}

	// ¸üÐÂ
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
	MapSceneAnimal::iterator it = m_SceneAnimalMap.find(sceneid);
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
	MapSceneAnimal::iterator it = m_SceneAnimalMap.find(sceneid);
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
Animal* Scene::GetSceneAnimal(int sceneid, uint64_t uid)
{
	MapSceneAnimal::iterator it = m_SceneAnimalMap.find(sceneid);
	if (it == m_SceneAnimalMap.end())
	{
		return nullptr;
	}

	std::map<uint64_t, Animal*>::iterator pos = it->second.find(uid);
	if (pos == it->second.end())
	{
		return nullptr;
	}

	return pos->second;
}

