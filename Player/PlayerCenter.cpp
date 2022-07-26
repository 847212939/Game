#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "../Game/stdafx.h"

PlayerCenter::PlayerCenter() : 
	m_SceneClient(nullptr)
{
	
}

PlayerCenter::~PlayerCenter()
{

}

void PlayerCenter::Init()
{
	if (!m_SceneClient)
	{
		COUT_LOG(LOG_CERROR, "Init scene = null");
		return;
	}
	PlayerPrepClient* playerPrepClient = m_SceneClient->GetPlayerPrepClient();
	if (!playerPrepClient)
	{
		COUT_LOG(LOG_CERROR, "Init sub player preproces = null");
		return;
	}
	TCPClient* pTCPClient = playerPrepClient->GetTCPClient();
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
void PlayerCenter::DispatchMessage(MsgCmd cmd, PlayerInfo* playerInfo)
{
	if (!m_SceneClient)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message scent = null cmd = %d", cmd);
		return;
	}
	PlayerPrepClient* playerPrepClient = m_SceneClient->GetPlayerPrepClient();
	if (!playerPrepClient)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message player preproces = null cmd = %d", cmd);
		return;
	}
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message playerClient info = null cmd = %d", cmd);
		return;
	}
	if (!playerInfo->m_pTcpSockInfo)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message sock info = null cmd = %d", cmd);
		return;
	}
	if (!playerInfo->m_pTcpSockInfo->isConnect)
	{
		COUT_LOG(LOG_CINFO, "Dispatch message Link broken cmd = %d", cmd);
		return;
	}
	if (!playerInfo->m_pMsg)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message sock msg = null cmd = %d", cmd);
		return;
	}
	PlayerClient* playerClient = GetPlayerClient(playerInfo->m_pMsg->uIndex);
	if (!playerClient)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message playerClient = null index = %u", playerInfo->m_pMsg->uIndex);
		return;
	}
	if (!playerClient->GetLoad())
	{
		COUT_LOG(LOG_CERROR, "Dispatch message mysql is unload index = %u", playerInfo->m_pMsg->uIndex);
		return;
	}
	if (strcmp(playerClient->GetTCPSocketInfo()->ip, playerInfo->m_pTcpSockInfo->ip) != 0)
	{
		COUT_LOG(LOG_CERROR, "The local IP address and remote IP address are not equal");
		return;
	}
	if (playerClient->GetIndex() != playerInfo->m_pMsg->uIndex)
	{
		COUT_LOG(LOG_CERROR, "dindex = %u, sindex = %u", playerClient->GetIndex(), playerInfo->m_pMsg->uIndex);
		return;
	}
	if (MsgCmd::MsgCmd_PlayerCenter == cmd)
	{
		playerPrepClient->CallBackFun((MsgCmd)playerInfo->m_pMsg->netMessageHead.uAssistantID, playerInfo);
	}
	else
	{
		playerClient->DispatchMessage(cmd, playerInfo);
	}
}

void PlayerCenter::CloseSocketEvent(unsigned int index)
{
	PlayerPrepClient* playerPrepClient = m_SceneClient->GetPlayerPrepClient();
	if (!playerPrepClient)
	{
		COUT_LOG(LOG_CERROR, "playerPrepClient = null index = %d", index);
		return;
	}
	TCPClient* pTCPClient = playerPrepClient->GetTCPClient();
	if (!pTCPClient)
	{
		COUT_LOG(LOG_CERROR, "pTCPClient = null index = %d", index);
		return;
	}

	pTCPClient->RemoveTCPSocketStatus(index);
}

// 玩家创建和数据库的加载
void PlayerCenter::HandlerPlayerThread()
{
	PlayerPrepClient* playerPrepClient = m_SceneClient->GetPlayerPrepClient();
	if (!playerPrepClient)
	{
		COUT_LOG(LOG_CERROR, "playerClient create thread err playerPrepClient = null");
		return;
	}
	TCPClient* pTCPClient = playerPrepClient->GetTCPClient();
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
				CloseSocketEvent(loadPKey.GetIndex());
				continue;
			}
			uint64_t userId = 0;
			LoginSys& loginSys = playerPrepClient->GetLoginSys();
			if (!loginSys.LoginIn(loadPKey.id, loadPKey.pw, userId))
			{
				CloseSocketEvent(loadPKey.GetIndex());
				continue;
			}
			if (userId == 0)
			{
				CloseSocketEvent(loadPKey.GetIndex());
				continue;
			}
			PlayerClient* playerClient = GetPlayerClient(loadPKey.GetIndex());
			if (playerClient)
			{
				new(playerClient) PlayerClient(loadPKey.GetIndex(), loadPKey.GetSocketInfo(), userId, playerPrepClient);
			}
			else
			{
				playerClient = new PlayerClient(loadPKey.GetIndex(), loadPKey.GetSocketInfo(), userId, playerPrepClient);
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
	m_SceneClient = pSubScene;
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
PlayerClient* PlayerCenter::GetPlayerClient(unsigned int index)
{
	if (index >= m_pPlayerVec.size() || index < 0)
	{
		return nullptr;
	}
	return m_pPlayerVec[index];
}

// 获取在线玩家
void PlayerCenter::GetSocketSet(std::vector<UINT>& socketVec)
{
	if (!m_SceneClient)
	{
		COUT_LOG(LOG_CERROR, "playerClient create thread err m_SceneClient = null");
		return;
	}
	PlayerPrepClient* playerPrepClient = m_SceneClient->GetPlayerPrepClient();
	if (!playerPrepClient)
	{
		COUT_LOG(LOG_CERROR, "playerClient create thread err playerPrepClient = null");
		return;
	}
	TCPClient* pTCPClient = playerPrepClient->GetTCPClient();
	if (!pTCPClient)
	{
		COUT_LOG(LOG_CERROR, "playerClient create thread err pTCPClient = null");
		return;
	}

	pTCPClient->GetSocketSet(socketVec);
}

// 获取场景
const SceneClient* PlayerCenter::GetSceneClient()
{
	return m_SceneClient;
}

ConditionVariable& PlayerCenter::GetConditionVariable()
{
	return m_cond;
}