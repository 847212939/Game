#pragma once

// �����Ϣ
class PlayerInfo
{
public:
	PlayerInfo();
	~PlayerInfo();

public:
	// SOCKET��ȡ֪ͨ�ṹ����
	SocketReadLine* m_pMsg;
	// ��ҷ��͹���������
	void*			m_pData;
	// ����������
	ServiceType		m_uSrverType;
	// ���id
	std::string		m_userId;
	// ���TCP��������Ϣ
	const TCPSocketInfo* m_pTcpSockInfo;
};
