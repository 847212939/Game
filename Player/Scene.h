#pragma once

class Scene
{
public:
	Scene();
	virtual ~Scene();

public:
	void Init();
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);
};
