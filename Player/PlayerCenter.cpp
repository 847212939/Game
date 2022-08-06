#include "../Game/stdafx.h"

PlayerCenter::PlayerCenter() : 
	m_pScene(nullptr)
{
	
}

PlayerCenter::~PlayerCenter()
{

}

void PlayerCenter::Init()
{
	if (!m_pScene)
	{
		COUT_LOG(LOG_CERROR, "Init scene = null");
		return;
	}
	PlayerPrepClient* pSubPlayerPreproces = m_pScene->GetPlayerPreproces();
	if (!pSubPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "Init sub player preproces = null");
		return;
	}
	TCPClient* pTCPClient = pSubPlayerPreproces->GetTCPClient();
	if (!pTCPClient)
	{
		COUT_LOG(LOG_CERROR, "Init tcp client = null");
		return;
	}

	CBaseCfgMgr& baseCfgMgr = CfgMgr()->GetCBaseCfgMgr();
	int maxSocketCnt = baseCfgMgr.GetMaxSocketCnt();

	// 初始化分配内存
	unsigned int playerSize = maxSocketCnt * 2;
	m_pPlayerVec.resize((size_t)playerSize);

	// 玩家全部初始化为空
	for (auto* playerClient : m_pPlayerVec)
	{
		playerClient = nullptr;
	}
	
	pTCPClient->GetSockeThreadVec().push_back(new std::thread(&PlayerCenter::HandlerPlayerThread, this));
}

// 分发消息
void PlayerCenter::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	if (!m_pScene)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message scent = null cmd = %d", cmd);
		return;
	}
	PlayerPrepClient* pSubPlayerPreproces = m_pScene->GetPlayerPreproces();
	if (!pSubPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message player preproces = null cmd = %d", cmd);
		return;
	}
	if (!pPlayerInfo)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message playerClient info = null cmd = %d", cmd);
		return;
	}
	if (!pPlayerInfo->m_pTcpSockInfo)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message sock info = null cmd = %d", cmd);
		return;
	}
	if (!pPlayerInfo->m_pTcpSockInfo->isConnect)
	{
		COUT_LOG(LOG_CINFO, "Dispatch message Link broken cmd = %d", cmd);
		return;
	}
	if (!pPlayerInfo->m_pMsg)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message sock msg = null cmd = %d", cmd);
		return;
	}
	PlayerClient* playerClient = GetSubPlayer(pPlayerInfo->m_pMsg->uIndex);
	if (!playerClient)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message playerClient = null index = %u", pPlayerInfo->m_pMsg->uIndex);
		return;
	}
	if (!playerClient->GetLoad())
	{
		COUT_LOG(LOG_CERROR, "Dispatch message mysql is unload index = %u", pPlayerInfo->m_pMsg->uIndex);
		return;
	}
	if (strcmp(playerClient->GetTCPSocketInfo()->ip, pPlayerInfo->m_pTcpSockInfo->ip) != 0)
	{
		COUT_LOG(LOG_CERROR, "The local IP address and remote IP address are not equal");
		return;
	}
	if (playerClient->GetIndex() != pPlayerInfo->m_pMsg->uIndex)
	{
		COUT_LOG(LOG_CERROR, "dindex = %u, sindex = %u", playerClient->GetIndex(), pPlayerInfo->m_pMsg->uIndex);
		return;
	}
	if (MsgCmd::MsgCmd_PlayerCenter == cmd)
	{
		pSubPlayerPreproces->CallBackFun((MsgCmd)pPlayerInfo->m_pMsg->netMessageHead.uAssistantID, pPlayerInfo);
	}
	else
	{
		playerClient->DispatchMessage(cmd, pPlayerInfo);
	}
}

// 玩家创建和数据库的加载
void PlayerCenter::HandlerPlayerThread()
{
	if (!m_pScene)
	{
		COUT_LOG(LOG_CERROR, "playerClient create thread err m_pScene = null");
		return;
	}
	PlayerPrepClient* pSubPlayerPreproces = m_pScene->GetPlayerPreproces();
	if (!pSubPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "playerClient create thread err pSubPlayerPreproces = null");
		return;
	}
	TCPClient* pTCPClient = pSubPlayerPreproces->GetTCPClient();
	if (!pTCPClient)
	{
		COUT_LOG(LOG_CERROR, "playerClient create thread err pTCPClient = null");
		return;
	}
	if (!pTCPClient->GetRuninged())
	{
		COUT_LOG(LOG_CERROR, "PlayerCenter::HandlerPlayerThread 初始化未完成");
		return;
	}
	LoadPlayerList& playerList = m_LoadPlayerList;
	bool& run = pTCPClient->GetRuninged();

	while (run)
	{
		//进入挂起状态
		std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
		m_cond.Wait(uniqLock, [&playerList, &run] { if (playerList.size() > 0 || !run) { return true; } return false; });

		if (playerList.size() <= 0)
		{
			uniqLock.unlock();
			continue;
		}

		LoadPlayerList loadPlayerList;
		loadPlayerList.swap(playerList);

		uniqLock.unlock();

		while (!loadPlayerList.empty())
		{
			LoadPlayerKey loadPKey = loadPlayerList.front();
			loadPlayerList.pop_front();
			 
			if (!loadPKey.GetConnect())
			{
				continue;
			}
			uint64_t userId = 0;
			LoginSys& loginSys = pSubPlayerPreproces->GetLoginSys();
			if (!loginSys.LoginIn(loadPKey.id, loadPKey.pw, userId))
			{
				continue;
			}
			if (userId == 0)
			{
				continue;
			}
			PlayerClient* playerClient = GetSubPlayer(loadPKey.GetIndex());
			if (playerClient)
			{
				new(playerClient) PlayerClient(loadPKey.GetIndex(), loadPKey.GetSocketInfo(), userId, pSubPlayerPreproces);
			}
			else
			{
				playerClient = new PlayerClient(loadPKey.GetIndex(), loadPKey.GetSocketInfo(), userId, pSubPlayerPreproces);
			}
			m_pPlayerVec[loadPKey.GetIndex()] = playerClient;

			playerClient->LoadMysql();
			playerClient->EnterGame();
			playerClient->RefreshProperties();
			playerClient->EnterScene();
			playerClient->SetLoad(true);
		}
	}

	COUT_LOG(LOG_CINFO, "playerClient create thread end...");
}

void PlayerCenter::SetSubScene(SceneClient* pSubScene)
{
	m_pScene = pSubScene;
}

// 创建角色
void PlayerCenter::CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& id, std::string& pw)
{
	m_cond.GetMutex().lock();
	m_LoadPlayerList.push_back(LoadPlayerKey(index, pSockInfo, id, pw));
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

// 获取玩家
PlayerClient* PlayerCenter::GetSubPlayer(unsigned int index)
{
	if (index >= m_pPlayerVec.size() || index < 0)
	{
		return nullptr;
	}
	return m_pPlayerVec[index];
}

// 获取在线玩家
const OnLinePlayerSet* PlayerCenter::GetSocketSet()
{
	if (!m_pScene)
	{
		COUT_LOG(LOG_CERROR, "playerClient create thread err m_pScene = null");
		return nullptr;
	}
	PlayerPrepClient* pSubPlayerPreproces = m_pScene->GetPlayerPreproces();
	if (!pSubPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "playerClient create thread err pSubPlayerPreproces = null");
		return nullptr;
	}
	TCPClient* pTCPClient = pSubPlayerPreproces->GetTCPClient();
	if (!pTCPClient)
	{
		COUT_LOG(LOG_CERROR, "playerClient create thread err pTCPClient = null");
		return nullptr;
	}

	return pTCPClient->GetSocketSet();
}

// 获取场景
const SceneClient* PlayerCenter::getScene()
{
	return m_pScene;
}

ConditionVariable& PlayerCenter::GetConditionVariable()
{
	return m_cond;
}