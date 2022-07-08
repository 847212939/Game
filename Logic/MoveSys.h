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
	void CallBackInit();
	void NetworkCallback(PlayerInfo* pPlayerInfo);
	void MysqlCallback(std::string&& data);

private:
	bool MoveCoo(CIstringstream& is, PlayerInfo* pPlayerInfo);

private:
	SubPlayer* m_pSubPlayer;
};
