#pragma once

class Monster : virtual public Animal
{
public:
	Monster();
	~Monster();

public:
	virtual AnimalType GetType();

public:
	GetSetMem(int, Monsterid)
};