#pragma once
class PlayerClient;

enum class MoveSysMsgCmd
{
	cs_move = 1,  // ÒÆ¶¯
};

class MoveSys
{
public:
	MoveSys(PlayerClient* playerClient);
	virtual~MoveSys();

private:
	void CalAttrs();
	void EnterScene();
	void LoadMysql(std::string& data);
	void NetWork(PlayerInfo* playerInfo);
	void Exit(SocketCloseLine* socketCloseLine);

private:
	bool MoveCoo(Netmsg& msg, PlayerInfo* playerInfo);

private:
	PlayerClient* m_PlayerClient;
};
