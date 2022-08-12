#pragma once

class PlayerClient;
class MoveSys
{
public:
	enum class MoveSysMsgCmd
	{
		cs_move = 1,  // ÒÆ¶¯
	};
public:
	MoveSys(PlayerClient* playerClient);
	virtual~MoveSys();

private:
	void LoadMysql(std::string& data);
	void CalAttrs();
	void EnterScene();
	void NetWork(PlayerInfo* playerInfo);
	void Exit(SocketCloseLine* socketCloseLine);

private:
	bool MoveCoo(Cis& is, PlayerInfo* playerInfo);

private:
	PlayerClient* m_PlayerClient;
};
