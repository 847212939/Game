#include "../Game/stdafx.h"

SubPlayerPreproces::SubPlayerPreproces(TCPClient* pTCPClient) :
	PlayerPreproces(pTCPClient),
	m_LoginSys(this)
{
	CallBackFunInit();
}

SubPlayerPreproces::~SubPlayerPreproces()
{
}

// ��ʼ����Ϣ�ص�����
void SubPlayerPreproces::CallBackFunInit()
{
}
