#pragma once

class PlayerClient;
class MoveSys
{
public:
	enum class MoveSysMsgCmd
	{
		CS_Move = 1,  // �ƶ�
	};
public:
	MoveSys(PlayerClient* playerClient);
	virtual~MoveSys();

private:
	void EnterGameCallback();
	void MysqlCallback(std::string& data);
	void NetworkCallback(PlayerInfo* playerInfo);
	void ExitCallback(SocketCloseLine* socketCloseLine);

private:
	bool MoveCoo(CIstringstream& is, PlayerInfo* playerInfo);

private:
	PlayerClient* m_PlayerClient;
};
