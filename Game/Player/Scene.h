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
	// 初始化
	void Init();
	// 进入场景
	bool EnterScene(Animal* animal, int sceneid, Transform transform);

public:
	// 添加动物到场景内
	void AddSceneAnimalMap(int sceneid, Animal* animal);
	// 删除场景内的动物
	void DelSceneAnimalMap(int sceneid, Animal* animal);
	// 根据id获取场景内动物
	Animal* GetSceneAnimal(int sceneid, uint64_t uid);

private:
	// 更新场景
	void UpdateScene(Animal* animal, int sceneid, Transform transform);

private:
	MapSceneAnimal	m_SceneAnimalMap;	// 根据场景添加动物
};
