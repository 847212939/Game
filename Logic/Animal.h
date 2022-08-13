#pragma once

class Animal
{
public:
	Animal();
	virtual~Animal();

public:
	virtual AnimalType GetType() = 0;

	GetSetMem(int, Sceneid)
	GetSetMem(uint64_t, ID)
	GetSetMem(Transform, transform)
	GetSetMem(bool, Lived)
};
