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
	int m_bmid;	// ������һ��ˢ������id
};
