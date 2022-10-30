#pragma once

class Animal
{
public:
	Animal();
	virtual~Animal();
	virtual AnimalType GetType() = 0;

public:
	// 设置属性
	void SetAttrValue(AttrsCmd attrType, int attr);
	// 添加属性
	void AdditionAttributes(MapAttrsCmdInt& attrs);
	// 获得属性
	int GetAttrValue(AttrsCmd attrType);
	// 刷新属性
	void RefreshProp();

public:
	// 根据技能位置确定技能配置
	const CSkillIdList* GetSkillIdListCfg(int skillpos);
	// 根据技能配置获取技能位置
	int GetSkillIdPos(const CSkillIdList* pCSkillIdList);

public:
	// 获取技能是否可以释放
	bool GetSkillAcitve(int pos);
	// 设置技能是否能激活
	void SetSkillAcitve(int pos, bool active);

public:
	BuildValue(int, Sceneid)				// 场景id
	BuildValue(uint64_t, ID)				// 唯一ID
	BuildValue(Transform, transform)		// 坐标
	BuildValue(bool, Lived)				// 是否复活
	BuildValue(time_t, ResuTime)			// 复活时间
	BuildValue(time_t, RefreshTime)		// 刷新时间
	BuildValue(int, Animalid)			// 动物id区分是什么动物
	BuildValue(HeroType, Animaltype)		// 英雄分类
	BuildValue(std::string, Animalname)	// 英雄name

	BuildValue(int, Silencetime)			// 技能沉默时间(单位毫秒/100)
	BuildValue(int, Striketime)			// 技能击飞时间(单位毫秒/100)
	BuildValue(int, Confinementtime)		// 技能禁锢时间(单位毫秒/100)

public:
	MapAttrsCmdInt	m_AttrsMap;				// 属性集合
	MapSkillData	m_SkillDataMap;			// 技能是否可以释放
};
