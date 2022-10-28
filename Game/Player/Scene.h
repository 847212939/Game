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
	// ��ʼ��
	void Init();
	// ���볡��
	bool EnterScene(Animal* animal, int sceneid, Transform transform);

public:
	// ��Ӷ��ﵽ������
	void AddSceneAnimalMap(int sceneid, Animal* animal);
	// ɾ�������ڵĶ���
	void DelSceneAnimalMap(int sceneid, Animal* animal);
	// ����id��ȡ�����ڶ���
	Animal* GetSceneAnimal(int sceneid, uint64_t uid);

private:
	// ���³���
	void UpdateScene(Animal* animal, int sceneid, Transform transform);

private:
	MapSceneAnimal	m_SceneAnimalMap;	// ���ݳ�����Ӷ���
};
