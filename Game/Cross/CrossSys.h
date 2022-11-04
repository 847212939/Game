#pragma once

enum class CrossSysMsgCmd
{
	cs_logic_to_cross_login		= 1,		// �������������¼
	cs_client_to_cross_logout	= 2,		// �˳����
	cs_logic_to_cross_close		= 3,		// �Է��ر���socket
	cs_cross_to_logic_logout	= 4,		// ������ص�¼���
};

class CrossSys
{
public:
	CrossSys(PlayerPrepClient* ppc);
	~CrossSys();

private:
	void Network(PlayerInfo* playerInfo);

private:
	// �ͻ��������¼���
	bool LogicToCrossLogin(Netmsg& msg, PlayerInfo* playerInfo);
	// ϵͳ�������ɶϿ�����
	bool CloseCross(Netmsg& msg, PlayerInfo* playerInfo);
};