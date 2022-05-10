#include "Log.h"
#include "TCPClient.h"
#include <thread>

TCPClient::TCPClient()
{
	Init(128, 8888);
	Start(SocketType::SOCKET_TYPE_TCP);
	Run();
}

TCPClient::~TCPClient()
{
	std::vector<std::thread*>& threadVec = GetSockeThreadVec();
	while (!threadVec.empty())
	{
		std::vector<std::thread*>::reverse_iterator it = threadVec.rbegin();
		(*it)->join();
		SafeDelete(*it);
	}
}

void TCPClient::Run()
{
	std::vector<std::thread*>& threadVec = GetSockeThreadVec();
	threadVec.push_back(new std::thread(&TCPClient::HandlerRecvDataList, this));
}

void TCPClient::HandlerRecvDataList()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));

	CDataLine* pDataLine = GetRecvDataLine();
	if (!pDataLine)
	{
		COUT_LOG(LOG_CERROR, "CDataLine error pDataLine == NULL");
		return;
	}

	//数据缓存
	void* pDataLineHead = NULL;

	COUT_LOG(LOG_CINFO, "TCPClient::HandlerRecvDataList thread begin...");

	while (GetRuninged())
	{
		//获取数据
		unsigned int bytes = pDataLine->GetData(&pDataLineHead);
		if (bytes == 0 || pDataLineHead == NULL)
		{
			continue;
		}

		//处理数据
		SocketReadLine* pMsg = reinterpret_cast<SocketReadLine*>(pDataLineHead);
		void* pData = static_cast<char*>(pDataLineHead) + sizeof(SocketReadLine);

		unsigned int index = pMsg->uIndex;
		unsigned int size = pMsg->uHandleSize;
		const std::vector<TCPSocketInfo>& socketInfoVec = GetSocketVector();

		if (index >= 0 && index < socketInfoVec.size())
		{
			const TCPSocketInfo& tcpInfo = socketInfoVec[index];
			PLAYER::PlayerInfo Info;
			Info.pMsg = pMsg;
			Info.pData = pData;
			Info.pTcpSockInfo = &tcpInfo;
			Info.uSrverType = GetServerType();
			m_RegisteredAccount.HandelMessage(&Info);
		}
		else
		{
			COUT_LOG(LOG_CERROR, "处理数据失败，index=%d 超出范围", index);
		}

		// 释放内存
		if (pDataLineHead)
		{
			SafeDeleteArray(pDataLineHead);
		}
	}

	COUT_LOG(LOG_CINFO, "TCPClient::HandlerRecvDataList exit...");

	return;
}
