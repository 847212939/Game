#pragma once

// 玩家信息
class PlayerInfo
{
public:
	PlayerInfo();
	virtual~PlayerInfo();

public:
	
	SocketReadLine* m_pMsg;					// SOCKET读取通知结构定义
	void*			m_pData;				// 玩家发送过来的数据
	ServiceType		m_uSrverType;			// 服务器类型
	uint64_t		m_userId;				// 玩家id
	const TCPSocketInfo* m_pTcpSockInfo;	// 玩家TCP的网络信息
};

struct LoadPlayerKey
{
	unsigned int			index;
	const TCPSocketInfo*	pSockInfo;
	uint64_t				userId;

	const bool& GetConnect();
	const unsigned int& GetIndex();
	const TCPSocketInfo* GetSocketInfo();
	const uint64_t& getUserId();

	LoadPlayerKey(int nIndex, const TCPSocketInfo* sockInfo, uint64_t& Id) : index(nIndex), pSockInfo(sockInfo), userId(Id) {}
	~LoadPlayerKey() {}
};

typedef std::map<std::string, std::string>							SqlKeyDataMap;	// 数据库查询结果
typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>>			NetFunMap;		// 消息回调函数
typedef std::map<std::string, std::function<void(std::string&&)>>	MysqlFunMap;	// 消息回调函数
