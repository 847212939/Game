#pragma once

class Animal
{
public:
	Animal();
	virtual~Animal();
	virtual AnimalType GetType() = 0;

public:
	// ����
	void AdditionAttributes(AttrsMap& attrs);
	void RefreshProp();

public:
	GetSetMem(int, Sceneid)				// ����id
	GetSetMem(uint64_t, ID)				// ΨһID
	GetSetMem(Transform, transform)		// ����
	GetSetMem(bool, Lived)				// �Ƿ񸴻�
	GetSetMem(uint64_t, ResuTime)		// ����ʱ��
	GetSetMem(uint64_t, RefreshTime)	// ˢ��ʱ��

public:
	AttrsMap m_AttrsMap;				// ���Լ���
};
