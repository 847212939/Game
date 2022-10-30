#include "../stdafx.h"

TCPClient::TCPClient() : CTCPSocketManage(), m_PlayerPrepClient(new PlayerPrepClient)
{
	RegisterNetType(TCPClient::SocketCallback, SysMsgCmd::HD_SOCKET_READ);
	RegisterNetType(TCPClient::TimerCallback, SysMsgCmd::HD_TIMER_MESSAGE);
	RegisterNetType(TCPClient::CloseSocketCallback, SysMsgCmd::HD_SOCKET_CLOSE);
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
	const CLogicCfg* pLogicCfg = GetServerCfg(serverType);
	if (!pLogicCfg)
	{
		return false;
	}
	int maxSocketCnt = G_CfgMgr->GetCBaseCfgMgr().GetMaxSocketCnt();
	if (!CTCPSocketManage::Init(maxSocketCnt, pLogicCfg->port, pLogicCfg->ip.c_str(), serverType))
	{
		return false;
	}
	if (!Start())
	{
		return false;
	}

	m_PlayerPrepClient->Init();
	GetSockeThreadVec().push_back(new std::thread(&TCPClient::HandlerRecvDataListThread, this));

	// ���ӷ�����
	if (!ConnectServer())
	{
		return false;
	}

	// �������ݿ�
	G_PlayerPrepClient->InitMysqlTable();

	// ע��ȫ�����ݿ�ص�����
	G_PlayerPrepClient->RegisterGlobalCallBack();
	G_SceneClient->RegisterGlobalCallBack();
	G_PlayerCenterClient->RegisterGlobalCallBack();

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
	CallBackFun((SysMsgCmd)pListItem->stDataHead.uDataKind, (void*)pListItem->pData);

	SafeDeleteArray(pListItem->pData);
	SafeDelete(pListItem);
}
void TCPClient::HandlerRecvDataListThread()
{
	bool& run = GetRuninged();
	std::list <ListItemData*> dataList;
	CDataLine* pDataLine = GetRecvDataLine();
	if (!pDataLine)
	{
		Log(CERR, "CDataLine error pDataLine == nullptr");
		return;
	}
	while (run)
	{
		if (!pDataLine->SwapDataList(dataList, run))
		{
			continue;
		}
		while (!dataList.empty())
		{
			HandleRecvData(dataList.front());
			dataList.pop_front();
		}
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
	CServerTimer* pCServerTimer = G_PlayerPrepClient->GetCServerTimer();
	if (!pCServerTimer)
	{
		Log(CERR, "pCServerTimer = null");
		return;
	}
	int timerCnt = G_CfgMgr->GetCBaseCfgMgr().GetTimerCnt();

	RecvDataLine->GetConditionVariable().NotifyAll();
	SendDataLine->GetConditionVariable().NotifyAll();
	G_PlayerCenterClient->GetConditionVariable().NotifyAll();

	for (int i = 0; i < timerCnt; i++)
	{
		pCServerTimer[i].SetTimerRun(false);
	}
}

void TCPClient::AddNetTypeCallback(SysMsgCmd cmd, std::function<void(void* pDataLineHead)>&& fun)
{
	MapTypeFunc::iterator it = m_TypeFunMap.find(cmd);
	if (it == m_TypeFunMap.end())
	{
		m_TypeFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	Log(CINF, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}
bool TCPClient::CallBackFun(SysMsgCmd cmd, void* pDataLineHead)
{
	MapTypeFunc::iterator it = m_TypeFunMap.find(cmd);
	if (it == m_TypeFunMap.end())
	{
		Log(CERR, "No corresponding callback function found cmd = %d", cmd);
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
		m_PlayerPrepClient->CallBackFun((TimerCmd)WindowTimer->uTimerID);
	}
	else
	{
		Log(CERR, "Timer message error");
	}
}
void TCPClient::SocketCallback(void* pDataLineHead)
{
	//��������
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
		Log(CERR, "Failed to process data��index=%d Out of range", index);
	}
}
void TCPClient::CloseSocketLogicCallback(void* pDataLineHead)
{
	SocketCloseLine* pSocketClose = (SocketCloseLine*)pDataLineHead;
	if (!pSocketClose)
	{
		return;
	}
	PlayerClient* playerClient = G_PlayerCenterClient->GetPlayerByIndex(pSocketClose->uIndex);
	if (!playerClient)
	{
		return;
	}
	if (pSocketClose->isCross)
	{
		playerClient->GetCrossClient().LogoutCross();
	}
	else
	{
		playerClient->ExitGame(pSocketClose);
	}

	VectorPlayerClient& playerClientVec = G_PlayerCenterClient->GetVectorPlayerClient();
	SafeDelete(playerClient);
	playerClientVec[pSocketClose->uIndex] = nullptr;
}
void TCPClient::CloseSocketCrossCallback(void* pDataLineHead)
{
	SocketCloseLine* pSocketClose = (SocketCloseLine*)pDataLineHead;
	if (!pSocketClose)
	{
		return;
	}
	PlayerClient* playerClient = G_PlayerCenterClient->GetPlayerByUserid(pSocketClose->userid);
	if (!playerClient)
	{
		return;
	}

	playerClient->ExitGame(pSocketClose);

	G_PlayerCenterClient->DelMapPlayerClient(pSocketClose->userid);
}
void TCPClient::CloseSocketCallback(void* pDataLineHead)
{
	GetServerType() == ServiceType::SERVICE_TYPE_CROSS ?
		CloseSocketCrossCallback(pDataLineHead) :
		CloseSocketLogicCallback(pDataLineHead);
}

const CLogicCfg* TCPClient::GetServerCfg(ServiceType serverType)
{
	if (serverType == ServiceType::SERVICE_TYPE_CROSS)
	{
		return &G_CfgMgr->GetCBaseCfgMgr().GetCrossServerCfg();
	}
	else
	{
		return &G_CfgMgr->GetCBaseCfgMgr().GetLogicCfg();
	}
}
bool TCPClient::IsCross()
{
	return GetServerType() == ServiceType::SERVICE_TYPE_CROSS;
}
