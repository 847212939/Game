#pragma once

class SubPlayerPreproces : public PlayerPreproces
{
public:
	~SubPlayerPreproces();
	SubPlayerPreproces(TCPClient* pTCPClient);

private:
	// ��ʼ����Ϣ�ص�����
	void CallBackFunInit();

private:
	LoginSys m_LoginSys;
};
