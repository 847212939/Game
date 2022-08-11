#pragma once

class Scene
{
public:
	Scene();
	virtual ~Scene();

protected:
	Scene(const Scene& my);
	Scene& operator=(const Scene& my);

public:
	void Init();
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);

private:
	SceneItemTypeMap m_SceneItemTypeMap;
};
