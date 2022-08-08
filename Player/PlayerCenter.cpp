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
	
	DTCPC->GetSockeThreadVec().push_back(new std::thread(&PlayerCenter::HandlerPlayerThread, this));
}

// 分发消息
void PlayerCenter::MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message playerClient info = null cmd = %d", cmd);
		return;
	}
	if (!playerInfo->pMsg)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message sock msg = null cmd = %d", cmd);
		return;
	}
	PlayerClient* playerClient = GetPlayerClientByIndex(playerInfo->pMsg->uIndex);
	if (!playerClient)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message playerClient = null index = %u", playerInfo->pMsg->uIndex);
		return;
	}
	const TCPSocketInfo* pInfo = DTCPC->GetTCPSocketInfo(playerInfo->pMsg->uIndex);
	if (!pInfo)
	{
		COUT_LOG(LOG_CERROR, "Client information is empty index=%d", playerInfo->pMsg->uIndex);
		return;
	}
	if (!pInfo->isConnect)
	{
		COUT_LOG(LOG_CINFO, "Dispatch message Link broken cmd = %d", cmd);
		return;
	}
	if (!playerClient->GetLoad())
	{
		COUT_LOG(LOG_CERROR, "Dispatch message mysql is unload index = %u", playerInfo->pMsg->uIndex);
		return;
	}
	if (playerClient->GetIndex() != playerInfo->pMsg->uIndex)
	{
		COUT_LOG(LOG_CERROR, "dindex = %u, sindex = %u", playerClient->GetIndex(), playerInfo->pMsg->uIndex);
		return;
	}
	if (MsgCmd::MsgCmd_PlayerCenter == cmd)
	{
		DPPC->CallBackFun((MsgCmd)playerInfo->pMsg->netMessageHead.uAssistantID, playerInfo);
	}
	else
	{
		playerClient->MessageDispatch(cmd, playerInfo);
	}
}

bool PlayerCenter::SwapLoadPlayerList(LoadPlayerList& LloadPlayerList, LoadPlayerList& RloadPlayerList, bool& run)
{
	RloadPlayerList.clear();

	std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
	m_cond.Wait(uniqLock, [&LloadPlayerList, &run] 
	{ 
		if (LloadPlayerList.size() > 0 || !run) 
		{ 
			return true; 
		} 
		return false; 
	});
	if (LloadPlayerList.size() <= 0)
	{
		uniqLock.unlock();
		return false;
	}

	RloadPlayerList.swap(LloadPlayerList);

	uniqLock.unlock();

	return true;
}

void PlayerCenter::HandleLoadPlayer(LoadPlayerKey& loadPKey)
{
	uint64_t userId = 0;

	const TCPSocketInfo* pInfo = DTCPC->GetTCPSocketInfo(loadPKey.index);
	if (!pInfo)
	{
		COUT_LOG(LOG_CERROR, "Client information is empty index=%d", loadPKey.index);
		return;
	}
	if (loadPKey.index < 0 || loadPKey.index >= m_PlayerClientVec.size())
	{
		DTCPC->CloseSocket(loadPKey.index);
		return;
	}
	if (!pInfo->isConnect)
	{
		DTCPC->CloseSocket(loadPKey.index);
		return;
	}
	if (!DPPC->GetLoginSys().LoginIn(loadPKey.id, loadPKey.pw, userId))
	{
		DTCPC->CloseSocket(loadPKey.index);
		return;
	}
	if (userId == 0)
	{
		DTCPC->CloseSocket(loadPKey.index);
		return;
	}
	PlayerClient* playerClient = GetPlayerClientByIndex(loadPKey.index);
	if (playerClient)
	{
		new(playerClient) PlayerClient(loadPKey.index, userId);
	}
	else
	{
		playerClient = new PlayerClient(loadPKey.index, userId);
	}
	m_PlayerClientVec[loadPKey.index] = playerClient;

	playerClient->LoadMysql();
	playerClient->EnterGame();
	playerClient->RefreshProperties();
	playerClient->EnterScene();
	playerClient->SetLoad(true);

	return;
}

void PlayerCenter::HandlerPlayerThread()
{
	bool& run = DTCPC->GetRuninged();

	LoadPlayerList loadPlayerList;
	LoadPlayerList& playerList = m_LoadPlayerList;

	while (run)
	{
		if (!SwapLoadPlayerList(playerList, loadPlayerList, run))
		{
			continue;
		}
		while (!loadPlayerList.empty())
		{
			HandleLoadPlayer(loadPlayerList.front());
			loadPlayerList.pop_front();
		}
	}
	COUT_LOG(LOG_CINFO, "playerClient create thread end...");
}

// 创建角色
void PlayerCenter::CreatePlayer(unsigned int index, std::string& id, std::string& pw)
{
	m_cond.GetMutex().lock();
	m_LoadPlayerList.push_back(LoadPlayerKey(index, id, pw));
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
	DTCPC->GetSocketSet(playerClientSet);
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
	DTCPC->GetSocketSet(socketVec);
}

ConditionVariable& PlayerCenter::GetConditionVariable()
{
	return m_cond;
}