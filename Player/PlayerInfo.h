#pragma once

// �����Ϣ
struct PlayerInfo
{
	PlayerInfo();
	~PlayerInfo();

	SocketReadLine* pMsg;
	void*			pData;
	ServiceType		uSrverType;
	const TCPSocketInfo* pTcpSockInfo;

};
