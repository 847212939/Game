#pragma once

class ActiveService
{
public:
	ActiveService();
	~ActiveService();

public:
	bool Enter(ActivityList* cfg);
	bool Exit(ActivityList* cfg);

private:

};
