#pragma once

class Animal
{
public:
	Animal();
	virtual~Animal();

public:
	virtual AnimalType GetType() = 0;
	virtual uint64_t GetID() = 0;
};
