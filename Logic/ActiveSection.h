#pragma once

// 时间内活动
class ActiveSection
{
public:
	ActiveSection(PlayerPrepClient* ppc);
	~ActiveSection();

public:
	bool Enter(ActivityList* cfg);
	bool Exit(ActivityList* cfg);

private:
	int m_bmid;
};
