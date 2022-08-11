#pragma once

class Animal
{
public:
	Animal();
	virtual~Animal();

public:
	virtual AnimalType GetType() = 0;

public:
	uint64_t GetID();
	void SetID(uint64_t id);
	int GetSceneid();
	void SetSceneid(int sceneid);
	const Transform& GetTransform();
	void SetTransform(Transform& transform);

private:
	uint64_t	m_Id;
	int			m_Sceneid;
	Transform	m_Transform;
};
