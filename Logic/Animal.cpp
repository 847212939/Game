#include "../Game/stdafx.h"

Animal::Animal()
{
}

Animal::~Animal()
{
}

uint64_t Animal::GetID()
{
	return m_Id;
}

void Animal::SetID(uint64_t id)
{
	m_Id = id;
}

int Animal::GetSceneid()
{
	return m_Sceneid;
}

void Animal::SetSceneid(int sceneid)
{
	m_Sceneid = sceneid;
}

const Transform& Animal::GetTransform()
{
	return m_Transform;
}

void Animal::SetTransform(Transform& transform)
{
	m_Transform = transform;
}