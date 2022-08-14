#pragma once

class ActiveTime
{
public:
	ActiveTime(PlayerPrepClient* ppc);
	~ActiveTime();

public:
	bool Enter(ActivityList* cfg);
	bool Exit(ActivityList* cfg);

private:
	int m_bmid;
};
