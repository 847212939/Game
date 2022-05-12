#include "../Game/stdafx.h"

TCPClient::TCPClient() : 
	m_pRegisteredAccount(new PlayerPreproces(this))
{
	Init(128, 8888);
	Start(ServiceType::SERVICE_TYPE_LOGIC);
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
	if (m_pRegisteredAccount)
	{
		SafeDelete(m_pRegisteredAccount);
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
			PlayerInfo Info;
			Info.m_pMsg = pMsg;
			Info.m_pData = pData;
			Info.m_pTcpSockInfo = &tcpInfo;
			Info.m_uSrverType = GetServerType();
			m_pRegisteredAccount->HandlerMessage(&Info);
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
