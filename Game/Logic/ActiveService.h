#pragma once

class ActiveService
{
public:
	ActiveService(PlayerPrepClient* ppc);
	~ActiveService();

public:
	bool Enter(CActivityList* cfg);
	bool Exit(CActivityList* cfg);

private:
	int m_bmid;	// 保存上一次刷怪配置id
};
