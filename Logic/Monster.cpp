#include "../Game/stdafx.h"

Monster::Monster(uint64_t id) : 
	Animal(),
	m_mid(id)
{
}

Monster::~Monster()
{
}

AnimalType Monster::GetType()
{
	return AnimalType::at_monster;
}

uint64_t Monster::GetID()
{
	return m_mid;
}