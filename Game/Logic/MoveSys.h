#pragma once
class PlayerClient;

enum class MoveSysMsgCmd
{
	cs_move = 1,		// �ƶ�
	cs_Load_mysql = 2,  // �������ݿ�
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
	bool LoadMysql(Netmsg& msg, PlayerInfo* playerInfo);

private:
	PlayerClient* m_PlayerClient;
};
