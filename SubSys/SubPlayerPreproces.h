#pragma once

class SubPlayerPreproces : public PlayerPreproces
{
public:
	// ע���¼��Э��
	enum class SubPPMsgCmd
	{
		CS_Login = 1,  // ��¼
	};
public:
	SubPlayerPreproces(TCPClient* pTCPClient);
	~SubPlayerPreproces();

private:
	// ��ʼ����Ϣ�ص�����
	void CallBackFunInit();

private:
	// ��¼
	void LoginInAccount(PlayerInfo* pPlayerInfo);
	bool LoginIn(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo);

private:
	std::string LoadUserAccount(std::string& id);
	std::string LoadUserId(std::string& id);

};
