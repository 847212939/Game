#pragma once

class Animal
{
public:
	Animal();
	virtual~Animal();
	virtual AnimalType GetType() = 0;

public:
	// 属性
	void AdditionAttributes(AttrsMap& attrs);
	void RefreshProp();

public:
	GetSetMem(int, Sceneid)				// 场景id
	GetSetMem(uint64_t, ID)				// 唯一ID
	GetSetMem(Transform, transform)		// 坐标
	GetSetMem(bool, Lived)				// 是否复活
	GetSetMem(uint64_t, ResuTime)		// 复活时间
	GetSetMem(uint64_t, RefreshTime)	// 刷新时间

public:
	AttrsMap m_AttrsMap;				// 属性集合
};
