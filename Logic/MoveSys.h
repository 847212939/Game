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
	void MysqlCallback(std::string&& data);
	void NetworkCallback(PlayerInfo* pPlayerInfo);
	void ExitCallback(SocketCloseLine* pPlayerInfo);

private:
	bool MoveCoo(CIstringstream& is, PlayerInfo* pPlayerInfo);

private:
	PlayerClient* m_PlayerClient;
};
