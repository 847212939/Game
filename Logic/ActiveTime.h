#pragma once

class ActiveTime
{
public:
	ActiveTime(PlayerPrepClient* ppc);
	~ActiveTime();

public:
	bool Enter(CActivityList* cfg);
	bool Exit(CActivityList* cfg);

private:
	int m_bmid;	// 保存上一次刷怪配置id
};
