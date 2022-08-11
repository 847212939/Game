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
	void AddAttrs();
	void EnterScene();
	void LoadMysql(std::string& data);
	void NetWork(PlayerInfo* playerInfo);
	void Exit(SocketCloseLine* socketCloseLine);

private:
	bool MoveCoo(Cis& is, PlayerInfo* playerInfo);

private:
	PlayerClient* m_PlayerClient;
};
