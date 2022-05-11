#pragma once

// 玩家信息
class PlayerInfo
{
public:
	PlayerInfo();
	~PlayerInfo();

public:
	// SOCKET读取通知结构定义
	SocketReadLine* m_pMsg;
	// 玩家发送过来的数据
	void*			m_pData;
	// 服务器类型
	ServiceType		m_uSrverType;
	// 玩家id
	std::string		m_userId;
	// 玩家TCP的网络信息
	const TCPSocketInfo* m_pTcpSockInfo;
};
