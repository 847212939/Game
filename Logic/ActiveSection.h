#pragma once

// ʱ���ڻ
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
