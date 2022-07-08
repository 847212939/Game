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
	long long		m_userId;				// 玩家id
	const TCPSocketInfo* m_pTcpSockInfo;	// 玩家TCP的网络信息
};

struct LoadPlayerKey
{
	unsigned int			index;
	const TCPSocketInfo*	pSockInfo;
	long long				userId;

	const bool& GetConnect();
	const unsigned int& GetIndex();
	const TCPSocketInfo* GetSocketInfo();
	const long long& getUserId();

	LoadPlayerKey(int nIndex, const TCPSocketInfo* sockInfo, long long& Id) : index(nIndex), pSockInfo(sockInfo), userId(Id) {}
	~LoadPlayerKey() {}
};

typedef std::map<std::string, std::string>							SqlKeyDataMap;	// 数据库查询结果
typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>>			NetFunMap;		// 消息回调函数
typedef std::map<std::string, std::function<void(std::string&)>>	MysqlFunMap;	// 消息回调函数
