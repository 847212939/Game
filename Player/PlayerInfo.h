#pragma once

// Íæ¼ÒÐÅÏ¢
struct PlayerInfo
{
	PlayerInfo();
	~PlayerInfo();

	SocketReadLine* pMsg;
	void*			pData;
	SocketType		uSrverType;
	const TCPSocketInfo* pTcpSockInfo;

};
