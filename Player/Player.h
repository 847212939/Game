#pragma once

// �����
class Player
{
public:

public:
	Player(PlayerInfo* pPlayerInfo);
	~Player();

public:
	// ��ȡ���id
	std::string GetUserId();
	// ��ȡ�����Ϣ
	PlayerInfo* GetPlayerInfo();

private:
	// �����Ϣ
	PlayerInfo* m_PlayerInfo;
};