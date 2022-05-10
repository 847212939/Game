#pragma once
#include <string>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <map>
#pragma comment(lib,"ws2_32.lib")

struct ClientItem
{
	bool isHandShark;
	SOCKET acceptid;
	ClientItem() :isHandShark(false), acceptid(INVALID_SOCKET){}
	~ClientItem(){}
};

class Server
{
	typedef std::map<SOCKET, ClientItem*> ClientItemMap;
public:
	Server(int port = 8080) {}
	virtual ~Server() {}

	static void writeFrameData(SOCKET sockid, std::string& data);
	void readMsg(SOCKET sockid);
	//static void write_msg_ret(uv_write_t *response, int status);

private:
	void safeCloseClient(ClientItem *cmsg = nullptr);

public:
	void pushClientItem(SOCKET sockid, ClientItem* item);
	bool isExistClient(SOCKET sockid);
	ClientItem* getClientItem(SOCKET sockid);
private:
	ClientItemMap m_clientItem;
};

