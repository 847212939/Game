#include "../Game/stdafx.h"

Monster::Monster() : 
	Animal()
{
}

Monster::~Monster()
{
}

AnimalType Monster::GetType()
{
	return AnimalType::at_monster;
}
