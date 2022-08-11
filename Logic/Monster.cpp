#include "../Game/stdafx.h"

Monster::Monster() : Animal()
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
	return m_ID;
}