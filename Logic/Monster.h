#pragma once

class Monster : virtual public Animal
{
public:
	Monster();
	~Monster();
	virtual AnimalType GetType();

public:
	// ��ʼ��
	void Init(int mid);
	// ����
	void Death();

public:
	GetSetMem(int, Monsterid)
};