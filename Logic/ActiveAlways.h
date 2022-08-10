#pragma once

class ActiveAlways
{
public:
	ActiveAlways();
	~ActiveAlways();

public:
	bool Enter(ActivityList* cfg);
	bool Exit(ActivityList* cfg);

private:

};
