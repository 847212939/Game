#pragma once

class ActiveTime
{
public:
	ActiveTime();
	~ActiveTime();

public:
	bool Enter(ActivityList* cfg);
	bool Exit(ActivityList* cfg);

private:

};
