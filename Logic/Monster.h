#pragma once

class Monster : virtual public Animal
{
public:
	Monster();
	~Monster();

public:
	virtual uint64_t GetID();
	virtual AnimalType GetType();

private:
	uint64_t m_ID;
};