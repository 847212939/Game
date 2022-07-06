#include "../Game/stdafx.h"

TCPClient::TCPClient() : 
	m_pRegisteredAccount(new PlayerPreproces(this))
{
	CBaseCfgMgr& baseCfgMgr = CfgMgr()->GetCBaseCfgMgr();
	const LogicCfg& logicCfg = baseCfgMgr.GetLogicCfg();

	Init(logicCfg.maxSocketCnt, logicCfg.port, logicCfg.ip.c_str());
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
	threadVec.push_back(new std::thread(&TCPClient::HandlerRecvDataListThread, this));
}

void TCPClient::HandlerRecvDataListThread()
{
	std::this_thread::sleep_for(std::chrono::seconds(2));

	CDataLine* pDataLine = GetRecvDataLine();
	if (!pDataLine)
	{
		COUT_LOG(LOG_CERROR, "CDataLine error pDataLine == NULL");
		return;
	}

	//���ݻ���
	void* pDataLineHead = NULL;

	COUT_LOG(LOG_CINFO, "recv data thread begin...");

	while (GetRuninged())
	{
		//��ȡ����
		unsigned int bytes = pDataLine->GetData(&pDataLineHead);
		if (bytes == 0 || pDataLineHead == NULL)
		{
			continue;
		}

		//��������
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
			COUT_LOG(LOG_CERROR, "��������ʧ�ܣ�index=%d ������Χ", index);
		}

		// �ͷ��ڴ�
		if (pDataLineHead)
		{
			SafeDeleteArray(pDataLineHead);
		}
	}

	COUT_LOG(LOG_CINFO, "recv data thread end...");

	return;
}
