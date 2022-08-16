#include "../Game/stdafx.h"

Animal::Animal() :
	m_Sceneid(0),
	m_ID(0),
	m_Lived(false),
	m_ResuTime(0),
	m_RefreshTime(0)
{
}

Animal::~Animal()
{

}

void Animal::AdditionAttributes(AttrsMap& attrs)
{
	for (AttrsMap::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		if ((AttrsCmd)it->first <= AttrsCmd::attrs_begin || (AttrsCmd)it->first >= AttrsCmd::attrs_end)
		{
			COUT_LOG(LOG_CERROR, "未知属性 请检查AttrsCmd.h头文件 属性为id:%d", it->first);
			continue;
		}
		m_AttrsMap[(AttrsCmd)it->first] += it->second;
	}
}

void Animal::RefreshProp()
{
	Cos os;
	os << (int)m_AttrsMap.size();
	for (AttrsMap::const_iterator it = m_AttrsMap.begin(); it != m_AttrsMap.end(); ++it)
	{
		os << (int)it->first << (int)it->second;
	}

	if (GetType() == AnimalType::at_player)
	{
		dynamic_cast<PlayerClient*>(this)->SendData(os.str().c_str(), os.str().size(), MsgCmd::MsgCmd_RefreshProperties, 1, 0);
	}
}

int Animal::GetAttrValue(AttrsCmd attrType)
{
	AttrsMap::iterator it = m_AttrsMap.find(attrType);
	if (it == m_AttrsMap.end())
	{
		return 0;
	}

	return it->second;
}

void Animal::SetAttrValue(AttrsCmd attrType, int attr)
{
	AttrsMap::iterator it = m_AttrsMap.find(attrType);
	if (it == m_AttrsMap.end())
	{
		m_AttrsMap.insert(std::make_pair(attrType, attr));
	}
	else
	{
		it->second += attr;
	}
}