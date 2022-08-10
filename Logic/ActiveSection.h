#pragma once

// 时间内活动
class ActiveSection
{
public:
	ActiveSection();
	~ActiveSection();

public:
	bool Enter(ActivityList* cfg);
	bool Exit(ActivityList* cfg);

private:

};
