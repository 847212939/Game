#pragma once

class Animal
{
public:
	Animal();
	virtual~Animal();

public:
	virtual AnimalType GetType() = 0;

public:
	int GetSceneid();
	uint64_t GetID();
	const Transform& GetTransform();
	void SetID(uint64_t id);
	void SetSceneid(int sceneid);
	void SetTransform(Transform& transform);

private:
	int			m_Sceneid;
	uint64_t	m_Id;
	Transform	m_Transform;
};
