#include "../Game/stdafx.h"

PlayerCenter::PlayerCenter(Scene* pScene) : 
	m_pScene(pScene)
{
	CBaseCfgMgr& baseCfgMgr = CfgMgr()->GetCBaseCfgMgr();
	const LogicCfg& logicCfg = baseCfgMgr.GetLogicCfg();

	// ��ʼ�������ڴ�
	unsigned int playerSize = logicCfg.maxSocketCnt * 2;
	m_pPlayerVec.resize((size_t)playerSize);

	// ���ȫ����ʼ��Ϊ��
	for (auto* player : m_pPlayerVec)
	{
		player = nullptr;
	}

	if (m_pScene)
	{
		PlayerPreproces* pPlayerPreproces = m_pScene->GetPlayerPreproces();
		if (pPlayerPreproces)
		{
			TCPClient* pTCPClient = pPlayerPreproces->GetTCPClient();
			if (pTCPClient)
			{
				std::vector<std::thread*>& threadVec = pTCPClient->GetSockeThreadVec();
				threadVec.push_back(new std::thread(&PlayerCenter::HandlerPlayerThread, this));
			}
		}
	}
}

PlayerCenter::~PlayerCenter()
{

}

// �ַ���Ϣ
void PlayerCenter::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	if (!pPlayerInfo)
	{
		COUT_LOG(LOG_CERROR, " Dispatch message player info = null");
		return;
	}
	if (!pPlayerInfo->m_pTcpSockInfo)
	{
		COUT_LOG(LOG_CERROR, " Dispatch message sock info = null");
		return;
	}
	if (!pPlayerInfo->m_pTcpSockInfo->isConnect)
	{
		COUT_LOG(LOG_CINFO, " Dispatch message Link broken");
		return;
	}
	Player* player = m_pPlayerVec[pPlayerInfo->m_pMsg->uIndex];
	if (!player)
	{
		COUT_LOG(LOG_CERROR, " Dispatch message player = null index = %u", pPlayerInfo->m_pMsg->uIndex);
		return;
	}
	if (strcmp(player->GetTCPSocketInfo()->ip, pPlayerInfo->m_pTcpSockInfo->ip) != 0)
	{
		COUT_LOG(LOG_CERROR, "The local IP address and remote IP address are not equal");
		return;
	}
	if (player->GetIndex() != pPlayerInfo->m_pMsg->uIndex)
	{
		COUT_LOG(LOG_CERROR, "The socket index received by the player is inconsistent "
			"player->GetIndex() = %u, "
			"pPlayerInfo->m_pMsg->uIndex = %u", 
			player->GetIndex(), 
			pPlayerInfo->m_pMsg->uIndex);
		return;
	}
	player->DispatchMessage(cmd, pPlayerInfo);
}

// ��Ҵ��������ݿ�ļ���
void PlayerCenter::HandlerPlayerThread()
{
	COUT_LOG(LOG_CINFO, "player create thread begin...");

	if (!m_pScene)
	{
		COUT_LOG(LOG_CERROR, "player create thread err m_pScene = null");
		return;
	}
	PlayerPreproces* pPlayerPreproces = m_pScene->GetPlayerPreproces();
	if (!pPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "player create thread err pPlayerPreproces = null");
		return;
	}
	TCPClient* pTCPClient = pPlayerPreproces->GetTCPClient();
	if (!pTCPClient)
	{
		COUT_LOG(LOG_CERROR, "player create thread err pTCPClient = null");
		return;
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));

	while (pTCPClient->GetRuninged())
	{
		//�������״̬
		std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
		m_cond.Wait(uniqLock, [this] { if (this->m_LoadPlayerList.size() > 0) { return true; } return false; });

		if (this->m_LoadPlayerList.size() <= 0)
		{
			continue;
		}

		LoadPlayerList loadPlayerList;
		std::swap(loadPlayerList, m_LoadPlayerList);

		uniqLock.unlock();

		while (!loadPlayerList.empty())
		{
			LoadPlayerKey loadPKey = loadPlayerList.front();
			loadPlayerList.pop_front();

			if (!loadPKey.pSockInfo->isConnect)
			{
				continue;
			}
			Player* player = m_pPlayerVec[loadPKey.index];
			if (player)
			{
				new(player) Player(loadPKey.index, loadPKey.pSockInfo, loadPKey.userId);
			}
			else
			{
				player = new Player(loadPKey.index, loadPKey.pSockInfo, loadPKey.userId);
			}

			player->SetPlayerPreproces(pPlayerPreproces);

			m_pPlayerVec[loadPKey.index] = player;

			player->CallBackFunInit();
			player->LoadMysql();
			player->EnterGame();
			player->EnterScene();
		}
	}

	COUT_LOG(LOG_CINFO, "player create thread end...");
}

// ������ɫ
bool PlayerCenter::CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId)
{
	m_cond.GetMutex().lock();
	m_LoadPlayerList.push_back(LoadPlayerKey(index, pSockInfo, userId));
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();

	return true;
}

// ��ȡ���
Player* PlayerCenter::GetPlayer(unsigned int index)
{
	return m_pPlayerVec[index];
}
