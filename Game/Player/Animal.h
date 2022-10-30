#pragma once

class Animal
{
public:
	Animal();
	virtual~Animal();
	virtual AnimalType GetType() = 0;

public:
	// ��������
	void SetAttrValue(AttrsCmd attrType, int attr);
	// �������
	void AdditionAttributes(MapAttrsCmdInt& attrs);
	// �������
	int GetAttrValue(AttrsCmd attrType);
	// ˢ������
	void RefreshProp();

public:
	// ���ݼ���λ��ȷ����������
	const CSkillIdList* GetSkillIdListCfg(int skillpos);
	// ���ݼ������û�ȡ����λ��
	int GetSkillIdPos(const CSkillIdList* pCSkillIdList);

public:
	// ��ȡ�����Ƿ�����ͷ�
	bool GetSkillAcitve(int pos);
	// ���ü����Ƿ��ܼ���
	void SetSkillAcitve(int pos, bool active);

public:
	BuildValue(int, Sceneid)				// ����id
	BuildValue(uint64_t, ID)				// ΨһID
	BuildValue(Transform, transform)		// ����
	BuildValue(bool, Lived)				// �Ƿ񸴻�
	BuildValue(time_t, ResuTime)			// ����ʱ��
	BuildValue(time_t, RefreshTime)		// ˢ��ʱ��
	BuildValue(int, Animalid)			// ����id������ʲô����
	BuildValue(HeroType, Animaltype)		// Ӣ�۷���
	BuildValue(std::string, Animalname)	// Ӣ��name

	BuildValue(int, Silencetime)			// ���ܳ�Ĭʱ��(��λ����/100)
	BuildValue(int, Striketime)			// ���ܻ���ʱ��(��λ����/100)
	BuildValue(int, Confinementtime)		// ���ܽ���ʱ��(��λ����/100)

public:
	MapAttrsCmdInt	m_AttrsMap;				// ���Լ���
	MapSkillData	m_SkillDataMap;			// �����Ƿ�����ͷ�
};
