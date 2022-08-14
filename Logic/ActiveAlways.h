#pragma once

class ActiveAlways
{
public:
	ActiveAlways(PlayerPrepClient* ppc);
	~ActiveAlways();

public:
	bool Enter(ActivityList* cfg);
	bool Exit(ActivityList* cfg);

private:
	int m_bmid;	// 保存上一次刷怪配置id
};
