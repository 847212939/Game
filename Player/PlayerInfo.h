#pragma once

// �����Ϣ
struct PlayerInfo
{
	PlayerInfo();
	~PlayerInfo();

	SocketReadLine* pMsg;
	void*			pData;
	SocketType		uSrverType;
	const TCPSocketInfo* pTcpSockInfo;

};
