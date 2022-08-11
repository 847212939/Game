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
	void EnterScene();
	void EnterGameCallback();
	void MysqlCallback(std::string& data);
	void NetworkCallback(PlayerInfo* playerInfo);
	void ExitCallback(SocketCloseLine* socketCloseLine);

private:
	bool MoveCoo(Cis& is, PlayerInfo* playerInfo);

private:
	PlayerClient* m_PlayerClient;
};
