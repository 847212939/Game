#pragma once

class ActiveAlways
{
public:
	ActiveAlways(PlayerPrepClient* ppc);
	~ActiveAlways();

public:
	bool Enter(CActivityList* cfg);
	bool Exit(CActivityList* cfg);

private:
	int m_bmid;	// ������һ��ˢ������id
};
