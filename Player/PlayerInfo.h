#pragma once

// Íæ¼ÒÐÅÏ¢
struct PlayerInfo
{
	PlayerInfo();
	~PlayerInfo();

	SocketReadLine* pMsg;
	void*			pData;
	ServiceType		uSrverType;
	const TCPSocketInfo* pTcpSockInfo;

};
