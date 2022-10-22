#pragma once

class Monster : virtual public Animal
{
public:
	Monster();
	~Monster();
	virtual AnimalType GetType();

public:
	// ³õÊ¼»¯
	void Init(int mid);
	// ËÀÍö
	void Death();

public:
	GetSetMem(int, Monsterid)
};