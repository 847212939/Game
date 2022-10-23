#pragma once
class PlayerClient;

enum class MoveSysMsgCmd
{
	cs_move = 1,  // 移动
	cs_Load = 2,  // 加载数据库
};

class MoveSys
{
public:
	MoveSys(PlayerClient* playerClient);
	virtual~MoveSys();

private:
	void CalAttrs();
	void EnterScene();
	void NetWork(PlayerInfo* playerInfo);
	void Exit(SocketCloseLine* socketCloseLine);

private:
	bool MoveCoo(Netmsg& msg, PlayerInfo* playerInfo);
	void LoadMysql(Netmsg& msg, PlayerInfo* playerInfo);

private:
	PlayerClient* m_PlayerClient;
};
