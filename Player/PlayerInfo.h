#pragma once

// 玩家信息
class PlayerInfo
{
public:
	PlayerInfo();
	~PlayerInfo();

public:
	
	SocketReadLine* m_pMsg;					// SOCKET读取通知结构定义
	void*			m_pData;				// 玩家发送过来的数据
	ServiceType		m_uSrverType;			// 服务器类型
	std::string		m_userId;				// 玩家id
	const TCPSocketInfo* m_pTcpSockInfo;	// 玩家TCP的网络信息
	bool			m_loadDbed;				// 加载玩家数据库标志
};
