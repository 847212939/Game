#include "../stdafx.h"

TCPClient::TCPClient() : CTCPSocketManage(), m_PlayerPrepClient(new PlayerPrepClient)
{
}
TCPClient::~TCPClient()
{
	std::vector<std::thread*>& threadVec = GetSockeThreadVec();
	while (!threadVec.empty())
	{
		std::vector<std::thread*>::iterator it = threadVec.begin();
		if (*it)
		{
			(*it)->join();
			SafeDelete(*it);
		}

		threadVec.erase(it);
	}
	if (m_PlayerPrepClient)
	{
		SafeDelete(m_PlayerPrepClient);
	}
}

bool TCPClient::Init(ServiceType serverType)
{
	const CServerCfg& serverCfg = G_CfgMgr->GetCBaseCfgMgr().GetServerCfg();
	int maxSocketCnt = G_CfgMgr->GetCBaseCfgMgr().GetMaxSocketCnt();

	if (!CTCPSocketManage::Init(maxSocketCnt, serverCfg.port, serverCfg.ip.c_str(), serverType))
	{
		return false;
	}
	if (!Start())
	{
		return false;
	}

	m_PlayerPrepClient->Init();

	GetSockeThreadVec().push_back(new std::thread(&TCPClient::HandlerRecvDataListThread, this));

	Log(CINF, "Server initialization succeeded");
	return true;
}

void TCPClient::HandleRecvData(ListItemData* pListItem)
{
	if (!pListItem)
	{
		return;
	}
	if (pListItem->stDataHead.uSize == 0 ||
		pListItem->pData == nullptr ||
		pListItem->stDataHead.uDataKind <= 0)
	{
		return;
	}
	SocketCallback((void*)pListItem->pData);

	SafeDeleteArray(pListItem->pData);
	SafeDelete(pListItem);
}
void TCPClient::HandlerRecvDataListThread()
{
	bool& run = GetRuninged();
	CDataLine* pDataLine = GetRecvDataLine();
	if (!pDataLine)
	{
		Log(CERR, "CDataLine error pDataLine == nullptr");
		return;
	}
	ListItemData* pListItem = NULL;
	unsigned int uDataKind = 0;
	while (true)
	{
		unsigned int bytes = pDataLine->GetData(&pListItem, run, uDataKind);
		if (bytes == 0 || pListItem == NULL)
		{
			continue;
		}
		HandleRecvData(pListItem);
	}

	Log(CINF, "recv data thread end");

	return;
}

PlayerPrepClient* TCPClient::GetPlayerPrepClient()
{
	return m_PlayerPrepClient;
}

void TCPClient::NotifyAll()
{
	CDataLine* RecvDataLine = GetRecvDataLine();
	CDataLine* SendDataLine = GetSendDataLine();
	if (!RecvDataLine)
	{
		Log(CERR, "RecvDataLine = null");
		return;
	}
	if (!SendDataLine)
	{
		Log(CERR, "SendDataLine = null");
		return;
	}
	
	RecvDataLine->GetConditionVariable().NotifyAll();
	SendDataLine->GetConditionVariable().NotifyAll();
	G_PlayerPrepClient->GetConditionVariable().NotifyAll();
}

void TCPClient::SocketCallback(void* pDataLineHead)
{
	//处理数据
	SocketReadLine* pMsg = reinterpret_cast<SocketReadLine*>(pDataLineHead);
	void* pData = static_cast<char*>(pDataLineHead) + sizeof(SocketReadLine);

	unsigned int index = pMsg->uIndex;
	unsigned int size = pMsg->uHandleSize;
	const std::vector<TCPSocketInfo>& socketInfoVec = GetSocketVector();

	if (index >= 0 && index < socketInfoVec.size())
	{
		PlayerInfo Info;
		Info.pMsg = pMsg;
		Info.pData = pData;
		Info.uSrverType = GetServerType();
		m_PlayerPrepClient->MessageDispatch(&Info);
	}
	else
	{
		Log(CERR, "Failed to process data，index=%d Out of range", index);
	}
}