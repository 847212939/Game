#include "../Game/stdafx.h"

TCPClient::TCPClient() : m_SubPlayerPreproces(new SubPlayerPreproces(this))
{
	
}

bool TCPClient::InitTCPClient()
{
	CBaseCfgMgr& baseCfgMgr = CfgMgr()->GetCBaseCfgMgr();
	const LogicCfg& logicCfg = baseCfgMgr.GetLogicCfg();
	int maxSocketCnt = baseCfgMgr.GetMaxSocketCnt();

	if (!Init(maxSocketCnt, logicCfg.port, logicCfg.ip.c_str()))
	{
		return false;
	}
	if (!Start(ServiceType::SERVICE_TYPE_LOGIC))
	{
		return false;
	}

	Run();

	COUT_LOG(LOG_CINFO, "Server initialization succeeded");
	return true;
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
	if (m_SubPlayerPreproces)
	{
		SafeDelete(m_SubPlayerPreproces);
	}
}

void TCPClient::Run()
{
	GetSockeThreadVec().push_back(new std::thread(&TCPClient::HandlerRecvDataListThread, this));
	m_SubPlayerPreproces->Init();
}

void TCPClient::HandlerRecvDataListThread()
{
	CDataLine* pDataLine = GetRecvDataLine();
	if (!pDataLine)
	{
		COUT_LOG(LOG_CERROR, "CDataLine error pDataLine == NULL");
		return;
	}
	if (!GetRuninged())
	{
		COUT_LOG(LOG_CERROR, "TCPClient::HandlerRecvDataListThread 初始化未完成");
		return;
	}

	//数据缓存
	void* pDataLineHead = NULL;

	bool& run = GetRuninged();

	while (run)
	{
		//获取数据
		unsigned int bytes = pDataLine->GetData(&pDataLineHead, run);
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
			m_SubPlayerPreproces->HandlerMessage(&Info);
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

	COUT_LOG(LOG_CINFO, "recv data thread end...");

	return;
}

SubPlayerPreproces* TCPClient::GetSubPlayerPreproces()
{
	return m_SubPlayerPreproces;
}

void TCPClient::NotifyAll()
{
	GetConditionVariable().NotifyAll();

	CDataLine* RecvDataLine = GetRecvDataLine();
	CDataLine* SendDataLine = GetSendDataLine();

	if (RecvDataLine && SendDataLine && m_SubPlayerPreproces)
	{
		RecvDataLine->GetConditionVariable().NotifyAll();
		SendDataLine->GetConditionVariable().NotifyAll();
		m_SubPlayerPreproces->GetConditionVariable().NotifyAll();
		m_SubPlayerPreproces->GetSubScene().GetPlayerCenter().GetConditionVariable().NotifyAll();
	}
}