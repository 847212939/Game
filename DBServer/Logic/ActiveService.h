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
	int m_bmid;	// ������һ��ˢ������id
};
