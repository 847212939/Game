#include "../Game/stdafx.h"

void Server::safeCloseClient(ClientItem *cmsg)
{
	if (cmsg == nullptr) return;
	ClientItemMap::iterator it = m_clientItem.find(cmsg->acceptid);
	if (it != m_clientItem.end())
	{
		m_clientItem.erase(it);
	}

	closesocket(cmsg->acceptid);
	delete cmsg;
	cmsg = nullptr;
}

void Server::readMsg(SOCKET sockid)
{
	ClientItemMap::iterator it = m_clientItem.find(sockid);
	if (it == m_clientItem.end())
	{
		safeCloseClient();
		return;
	}
	ClientItem* cmsg = it->second;
	if (!cmsg)
	{
		safeCloseClient(cmsg);
		return;
	}
	if (cmsg == nullptr)
	{
		safeCloseClient(cmsg);
		return;
	}
	if (cmsg->acceptid == INVALID_SOCKET)
	{
		safeCloseClient(cmsg);
		return;
	}

	while (true)
	{
		WebSocket msg;
		const int nlen = 1024 * 4;
		char recvbuf[nlen] = "";
		int nsize = recv(cmsg->acceptid, recvbuf, nlen - 1, 0);
		if (nsize <= 0)
		{
			safeCloseClient(cmsg);
			return;
		}
		if (!cmsg->isHandShark)
		{
			bool rc = msg.FromHandshakePkt(nsize, recvbuf);
			if (!rc)
			{
				safeCloseClient(cmsg);
				return;
			}
			else
			{
				std::string&& pkt = msg.ToHandshakePkt();
				if (pkt.empty())
				{
					safeCloseClient(cmsg);
					return;
				}
				else
				{
					writeFrameData(cmsg->acceptid, pkt);
					cmsg->isHandShark = true;
				}
			}
		}
		else
		{
			int rdp = msg.FromFrameDataPkt(nsize, recvbuf);
			if (rdp > 0)
			{
				std::string str = msg.RequestData();

				WebSocket::WebSocketPkt::MsgType type = msg.RequestOpcode();
				if (type == (WebSocket::WebSocketPkt::MsgType)WebSocket::WebSocketPkt::OpcodeType::OpcodeType_Close)
				{
					safeCloseClient(cmsg);
					return;
				}
				else if (type == (WebSocket::WebSocketPkt::MsgType)WebSocket::WebSocketPkt::OpcodeType::OpcodeType_Ping)
				{
					WebSocket msg;
					std::string data = msg.ToFrameDataPkt("", WebSocket::WebSocketPkt::OpcodeType::OpcodeType_Pong);
					if (cmsg)
					{
						writeFrameData(cmsg->acceptid, data);
					}
				}
				else if (type == (WebSocket::WebSocketPkt::MsgType)WebSocket::WebSocketPkt::OpcodeType::OpcodeType_Binary)
				{
					std::string data = msg.ToFrameDataPkt(str, WebSocket::WebSocketPkt::OpcodeType::OpcodeType_Binary);
					if (cmsg)
					{
						writeFrameData(cmsg->acceptid, data);
					}
				}
				else if (type == (WebSocket::WebSocketPkt::MsgType)WebSocket::WebSocketPkt::OpcodeType::OpcodeType_Text)
				{
					std::string data = msg.ToFrameDataPkt(str, WebSocket::WebSocketPkt::OpcodeType::OpcodeType_Text);
					if (cmsg)
					{
						writeFrameData(cmsg->acceptid, data);
					}
				}
			}
		}
	}
}

void Server::writeFrameData(SOCKET sockid, std::string& data)
{
	send(sockid, data.c_str(), (int)data.size(), 0);
}

void Server::pushClientItem(SOCKET sockid, ClientItem* item)
{
	m_clientItem.insert(std::make_pair(sockid, item));
}

bool Server::isExistClient(SOCKET sockid)
{
	ClientItemMap::iterator it = m_clientItem.find(sockid);
	if (it != m_clientItem.end())
	{
		return true;
	}
	return false;
}

ClientItem* Server::getClientItem(SOCKET sockid)
{
	ClientItemMap::iterator it = m_clientItem.find(sockid);
	if (it != m_clientItem.end())
	{
		return it->second;
	}
	return nullptr;
}
