#pragma once

// ʱ���ڻ
class ActiveSection
{
public:
	ActiveSection(PlayerPrepClient* ppc);
	~ActiveSection();

public:
	bool Enter(CActivityList* cfg);
	bool Exit(CActivityList* cfg);

private:
	int m_bmid;	// ������һ��ˢ������id
};
