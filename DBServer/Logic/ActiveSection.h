#pragma once

// 时间内活动
class ActiveSection
{
public:
	ActiveSection(PlayerPrepClient* ppc);
	~ActiveSection();

public:
	bool Enter(CActivityList* cfg);
	bool Exit(CActivityList* cfg);

private:
	int m_bmid;	// 保存上一次刷怪配置id
};
