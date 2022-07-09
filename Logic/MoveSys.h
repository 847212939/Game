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
	~MoveSys();

private:
	void Register();
	void MysqlCallback(std::string&& data);
	void EnterGameCallback(AttrsMap& attrs);
	void NetworkCallback(PlayerInfo* pPlayerInfo);

private:
	bool MoveCoo(CIstringstream& is, PlayerInfo* pPlayerInfo);

private:
	SubPlayer* m_pSubPlayer;
};
