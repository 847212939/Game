#pragma once

class SubPlayerPreproces : public PlayerPreproces
{
public:
	~SubPlayerPreproces();
	SubPlayerPreproces(TCPClient* pTCPClient);

private:
	// 初始化消息回调函数
	void CallBackFunInit();

private:
	LoginSys m_LoginSys;
};
