#pragma once

class Animal
{
public:
	Animal();
	virtual~Animal();
	virtual AnimalType GetType() = 0;

public:
	// 属性
	void SetAttrValue(AttrsCmd attrType, int attr);
	void AdditionAttributes(AttrsMap& attrs);
	int GetAttrValue(AttrsCmd attrType);
	void RefreshProp();

public:
	// 技能
	const CSkillIdList* GetSkillIdListCfg(int skillpos);
	int GetSkillIdPos(const CSkillIdList* pCSkillIdList);

public:
	GetSetMem(int, Sceneid)				// 场景id
	GetSetMem(uint64_t, ID)				// 唯一ID
	GetSetMem(Transform, transform)		// 坐标
	GetSetMem(bool, Lived)				// 是否复活
	GetSetMem(time_t, ResuTime)			// 复活时间
	GetSetMem(time_t, RefreshTime)		// 刷新时间
	GetSetMem(int, Animalid)			// 动物id区分是什么动物
	GetSetMem(HeroType, Animaltype)		// 英雄分类
	GetSetMem(std::string, Animalname)	// 英雄name

	GetSetMem(int, Silencetime)			// 技能沉默时间(单位毫秒/100)
	GetSetMem(int, Striketime)			// 技能击飞时间(单位毫秒/100)
	GetSetMem(int, Confinementtime)		// 技能禁锢时间(单位毫秒/100)

public:
	GetClass(SkillSys, SkillSys)		// 技能系统

public:
	AttrsMap m_AttrsMap;				// 属性集合
};
