#pragma once

// �����Ϣ
class PlayerInfo
{
public:
	PlayerInfo();
	~PlayerInfo();

public:
	
	SocketReadLine* m_pMsg;					// SOCKET��ȡ֪ͨ�ṹ����
	void*			m_pData;				// ��ҷ��͹���������
	ServiceType		m_uSrverType;			// ����������
	std::string		m_userId;				// ���id
	const TCPSocketInfo* m_pTcpSockInfo;	// ���TCP��������Ϣ
	bool			m_loadDbed;				// ����������ݿ��־
};
