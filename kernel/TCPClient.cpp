#include "../Game/stdafx.h"

TCPClient::TCPClient() : m_SubPlayerPreproces(new SubPlayerPreproces(this))
{
	RegisterType(this, TCPClient::SocketCallback, HD_SOCKET_READ);
	RegisterType(this, TCPClient::TimerCallback, HD_TIMER_MESSAGE);
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

	m_SubPlayerPreproces->Init();

	GetSockeThreadVec().push_back(new std::thread(&TCPClient::HandlerRecvDataListThread, this));

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

	void* pDataLineHead = NULL;
	bool& run = GetRuninged();

	while (run)
	{
		unsigned int uDataKind = 0;
		unsigned int bytes = pDataLine->GetData(&pDataLineHead, run, uDataKind);
		if (bytes == 0 || pDataLineHead == NULL || uDataKind <= 0)
		{
			continue;
		}
		
		CallBackFun(uDataKind, pDataLineHead);

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
	if (!RecvDataLine)
	{
		COUT_LOG(LOG_CERROR, "RecvDataLine = null");
		return;
	}
	if (!SendDataLine)
	{
		COUT_LOG(LOG_CERROR, "SendDataLine = null");
		return;
	}
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "SubPlayerPreproces = null");
		return;
	}
	CServerTimer* pCServerTimer = m_SubPlayerPreproces->GetCServerTimer();
	if (!pCServerTimer)
	{
		COUT_LOG(LOG_CERROR, "pCServerTimer = null");
		return;
	}
	CBaseCfgMgr& baseCfgMgr = CfgMgr()->GetCBaseCfgMgr();
	int timerCnt = baseCfgMgr.GetTimerCnt();

	RecvDataLine->GetConditionVariable().NotifyAll();
	SendDataLine->GetConditionVariable().NotifyAll();
	m_SubPlayerPreproces->GetSubScene().GetPlayerCenter().GetConditionVariable().NotifyAll();

	for (int i = 0; i < timerCnt; i++)
	{
		pCServerTimer[i].SetTimerRun(false);
	}
}

void TCPClient::AddTypeCallback(int cmd, std::function<void(void* pDataLineHead)>&& fun)
{
	TypeFunMap::iterator it = m_TypeFunMap.find(cmd);
	if (it == m_TypeFunMap.end())
	{
		m_TypeFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CINFO, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

bool TCPClient::CallBackFun(int cmd, void* pDataLineHead)
{
	TypeFunMap::iterator it = m_TypeFunMap.find(cmd);
	if (it == m_TypeFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", cmd);
		return false;
	}

	it->second(pDataLineHead);
	return true;
}

void TCPClient::TimerCallback(void* pDataLineHead)
{
	ServerTimerLine* WindowTimer = (ServerTimerLine*)pDataLineHead;
	if (WindowTimer->uMainID == (unsigned int)MsgCmd::MsgCmd_Timer)
	{
		m_SubPlayerPreproces->CallBackFun((TimerCmd)WindowTimer->uTimerID);
	}
	else
	{
		COUT_LOG(LOG_CERROR, "Timer message error");
	}
}

void TCPClient::SocketCallback(void* pDataLineHead)
{
	//处理数据
	SocketReadLine* pMsg = reinterpret_cast<SocketReadLine*>(pDataLineHead);
	void* pData = static_cast<char*>(pDataLineHead) + sizeof(SocketReadLine);

	unsigned int index = pMsg->uIndex;
	unsigned int size = pMsg->uHandleSize;
	const std::vector<TCPSocketInfo>& socketInfoVec = GetSocketVector();

	COUT_LOG(LOG_CINFO, "index = %d, size = %d", index, (int)socketInfoVec.size());

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
		COUT_LOG(LOG_CERROR, "Failed to process data，index=%d Out of range", index);
	}
}

