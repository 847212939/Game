#pragma once

class Monster : virtual public Animal
{
public:
	Monster();
	~Monster();

public:
	virtual AnimalType GetType();

public:
	void Init(int mid);

public:
	GetSetMem(int, Monsterid)
};