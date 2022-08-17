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
	// ����
	const CSkillIdList* GetSkillIdListCfg(int skillpos);
	int GetSkillIdPos(const CSkillIdList* pCSkillIdList);

public:
	GetSetMem(int, Sceneid)				// ����id
	GetSetMem(uint64_t, ID)				// ΨһID
	GetSetMem(Transform, transform)		// ����
	GetSetMem(bool, Lived)				// �Ƿ񸴻�
	GetSetMem(time_t, ResuTime)			// ����ʱ��
	GetSetMem(time_t, RefreshTime)		// ˢ��ʱ��
	GetSetMem(int, Animalid)			// ����id������ʲô����
	GetSetMem(HeroType, Animaltype)		// Ӣ�۷���
	GetSetMem(std::string, Animalname)	// Ӣ��name

	GetSetMem(int, Silencetime)			// ���ܳ�Ĭʱ��(��λ����/100)
	GetSetMem(int, Striketime)			// ���ܻ���ʱ��(��λ����/100)
	GetSetMem(int, Confinementtime)		// ���ܽ���ʱ��(��λ����/100)

public:
	GetClass(SkillSys, SkillSys)		// ����ϵͳ

public:
	AttrsMap m_AttrsMap;				// ���Լ���
};
