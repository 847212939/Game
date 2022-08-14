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

void Monster::Init(int mid)
{
	SetID(DUtil.CreateUserId());
	SetMonsterid(mid);
}

void Monster::Death()
{
	SetLived(false);
	SetResuTime(::time(nullptr) + GetRefreshTime());
}