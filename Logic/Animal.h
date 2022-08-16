#pragma once

class Animal
{
public:
	Animal();
	virtual~Animal();
	virtual AnimalType GetType() = 0;

public:
	// ����
	void SetAttrValue(AttrsCmd attrType, int attr);
	void AdditionAttributes(AttrsMap& attrs);
	int GetAttrValue(AttrsCmd attrType);
	void RefreshProp();

public:
	GetSetMem(int, Sceneid)				// ����id
	GetSetMem(uint64_t, ID)				// ΨһID
	GetSetMem(Transform, transform)		// ����
	GetSetMem(bool, Lived)				// �Ƿ񸴻�
	GetSetMem(time_t, ResuTime)			// ����ʱ��
	GetSetMem(time_t, RefreshTime)		// ˢ��ʱ��

public:
	AttrsMap m_AttrsMap;				// ���Լ���
};
