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
};
