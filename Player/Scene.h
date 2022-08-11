#pragma once

class Scene
{
public:
	Scene();
	virtual ~Scene();

protected:
	Scene(const Scene& my);
	Scene& operator=(const Scene& my);

public:
	void Init();
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);

public:
	bool EnterScene(Animal* animal, int sceneid, Transform& transform);

private:
	void AddSceneAnimalMap(int sceneid, Animal* animal);
	void DelSceneAnimalMap(int sceneid, Animal* animal);

private:
	SceneAnimalMap m_SceneAnimalMap;	// 根据场景添加动物
};
