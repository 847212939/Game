#pragma once

enum class CrossClientMsgCmd
{
	cs_logic_to_cross_login		= 1,		// �������������¼
	cs_client_to_cross_logout	= 2,		// �˳����
	cs_logic_to_cross_close		= 3,		// �Է��ر���socket
	cs_cross_to_logic_logout	= 4,		// ������ص�¼���
};

class CrossClient
{
public:
	CrossClient(PlayerClient* player);
	~CrossClient();

public:
	void Network(PlayerInfo* playerInfo);

public:
	// ϵͳ�������ɶϿ�����
	bool CloseCross();

private:
	// �ͻ��������¼���
	bool LogicToCrossLogin(Netmsg& msg, PlayerInfo* playerInfo);
	// �ͻ�������Ͽ�����
	bool ClientToCrossLogout(Netmsg& msg, PlayerInfo* playerInfo);

private:
	PlayerClient* m_Player;
};