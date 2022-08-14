#pragma once

class ActiveService
{
public:
	ActiveService(PlayerPrepClient* ppc);
	~ActiveService();

public:
	bool Enter(ActivityList* cfg);
	bool Exit(ActivityList* cfg);

private:
	int m_bmid;
};
