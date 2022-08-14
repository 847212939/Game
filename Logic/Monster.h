#pragma once

class Monster : virtual public Animal
{
public:
	Monster();
	~Monster();
	virtual AnimalType GetType();

public:
	void Init(int mid);
	void Death();

public:
	GetSetMem(int, Monsterid)
};