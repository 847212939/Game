#pragma once

class SubPlayer;
class MoveSys
{
public:
	enum class MoveSysMsgCmd
	{
		CS_Move = 1,  // ÒÆ¶¯
	};
public:
	MoveSys(SubPlayer* pSubPlayer);
	virtual~MoveSys();

private:
	void EnterGameCallback();
	void MysqlCallback(std::string&& data);
	void NetworkCallback(PlayerInfo* pPlayerInfo);
	void ExitCallback(SocketCloseLine* pPlayerInfo);

private:
	bool MoveCoo(CIstringstream& is, PlayerInfo* pPlayerInfo);

private:
	SubPlayer* m_pSubPlayer;
};
