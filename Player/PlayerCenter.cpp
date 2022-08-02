#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "../Game/stdafx.h"

PlayerCenter::PlayerCenter()
{
	
}

PlayerCenter::~PlayerCenter()
{

}

void PlayerCenter::Init()
{
	int maxSocketCnt = BaseCfgMgr.GetMaxSocketCnt();

	// 初始化分配内存
	unsigned int playerSize = maxSocketCnt * 2;
	m_PlayerClientVec.resize((size_t)playerSize);

	// 玩家全部初始化为空
	for (auto* playerClient : m_PlayerClientVec)
	{
		playerClient = nullptr;
	}
	
	DTCPClient->GetSockeThreadVec().push_back(new std::thread(&PlayerCenter::HandlerPlayerThread, this));
}

// 分发消息
void PlayerCenter::MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo)
{
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
	PlayerClient* playerClient = GetPlayerClientByIndex(playerInfo->m_pMsg->uIndex);
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
		DPlayerPrepClient->CallBackFun((MsgCmd)playerInfo->m_pMsg->netMessageHead.uAssistantID, playerInfo);
	}
	else
	{
		playerClient->MessageDispatch(cmd, playerInfo);
	}
}

// 玩家创建和数据库的加载
void PlayerCenter::HandlerPlayerThread()
{
	if (!DTCPClient->GetRuninged())
	{
		COUT_LOG(LOG_CERROR, "PlayerCenter::HandlerPlayerThread 初始化未完成");
		return;
	}
	LoadPlayerList& playerList = m_LoadPlayerList;
	bool& run = DTCPClient->GetRuninged();

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
			uint64_t userId = 0;

			if (loadPKey.GetIndex() < 0 || loadPKey.GetIndex() >= m_PlayerClientVec.size())
			{
				DTCPClient->CloseSocket(loadPKey.GetIndex());
				continue;
			}
			if (!loadPKey.GetConnect())
			{
				DTCPClient->CloseSocket(loadPKey.GetIndex());
				continue;
			}
			if (!DPlayerPrepClient->GetLoginSys().LoginIn(loadPKey.id, loadPKey.pw, userId))
			{
				DTCPClient->CloseSocket(loadPKey.GetIndex());
				continue;
			}
			if (userId == 0)
			{
				DTCPClient->CloseSocket(loadPKey.GetIndex());
				continue;
			}
			PlayerClient* playerClient = GetPlayerClientByIndex(loadPKey.GetIndex());
			if (playerClient)
			{
				new(playerClient) PlayerClient(loadPKey.GetIndex(), loadPKey.GetSocketInfo(), userId);
			}
			else
			{
				playerClient = new PlayerClient(loadPKey.GetIndex(), loadPKey.GetSocketInfo(), userId);
			}
			m_PlayerClientVec[loadPKey.GetIndex()] = playerClient;

			playerClient->LoadMysql();
			playerClient->EnterGame();
			playerClient->RefreshProperties();
			playerClient->EnterScene();
			playerClient->SetLoad(true);
		}
	}

	COUT_LOG(LOG_CINFO, "playerClient create thread end...");
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
PlayerClient* PlayerCenter::GetPlayerClientByIndex(unsigned int index)
{
	if (index >= m_PlayerClientVec.size() || index < 0)
	{
		return nullptr;
	}
	return m_PlayerClientVec[index];
}

PlayerClient* PlayerCenter::GetPlayerClientByUserid(uint64_t userId)
{
	std::vector<UINT> playerClientSet;
	DTCPClient->GetSocketSet(playerClientSet);
	for (int index : playerClientSet)
	{
		if (index >= m_PlayerClientVec.size() || index < 0)
		{
			continue;
		}
		PlayerClient* playerClient = m_PlayerClientVec[index];
		if (!playerClient)
		{
			continue;
		}
		if (playerClient->GetUserId() == userId)
		{
			return playerClient;
		}
	}

	return nullptr;
}

// 获取在线玩家
void PlayerCenter::GetSocketSet(std::vector<UINT>& socketVec)
{
	DTCPClient->GetSocketSet(socketVec);
}

ConditionVariable& PlayerCenter::GetConditionVariable()
{
	return m_cond;
}