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
	PlayerInfo* m_PlayerInfo;   // �����Ϣ
	PlayerAttrs m_PlayerAttrs;  // �������
};