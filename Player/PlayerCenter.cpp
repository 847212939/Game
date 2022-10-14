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
	if (MsgCmd::MsgCmd_PlayerCenter == (MsgCmd)playerInfo->pMsg->netMessageHead.uIdentification)
	{
		DPPC->CallBackFun(cmd, playerInfo);
	}
	else
	{
		playerClient->MessageDispatch(cmd, playerInfo);
	}
}

// 创建角色
void PlayerCenter::CreatePlayer(LoginData& loginData)
{
	m_cond.GetMutex().lock();
	m_LoadPlayerList.push_back(loginData);
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
	std::vector<unsigned int> playerClientSet;
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
		if (playerClient->GetID() == userId)
		{
			return playerClient;
		}
	}

	return nullptr;
}
void PlayerCenter::GetSocketSet(std::vector<unsigned int>& socketVec)
{
	// 获取在线玩家
	DTCPC->GetSocketSet(socketVec);
}
ConditionVariable& PlayerCenter::GetConditionVariable()
{
	return m_cond;
}

// 玩家加载线程
bool PlayerCenter::SwapLoadPlayerList(ListLoginData& LloadPlayerList, ListLoginData& RloadPlayerList, bool& run)
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
void PlayerCenter::HandleLoadPlayer(LoginData& loginData)
{
	const TCPSocketInfo* pInfo = DTCPC->GetTCPSocketInfo(loginData.index);
	if (!pInfo)
	{
		COUT_LOG(LOG_CERROR, "Client information is empty index=%d", loginData.index);
		return;
	}
	if (loginData.index < 0 || loginData.index >= m_PlayerClientVec.size())
	{
		DTCPC->CloseSocket(loginData.index);
		return;
	}
	if (!pInfo->isConnect)
	{
		DTCPC->CloseSocket(loginData.index);
		return;
	}
	PlayerClient* playerClient = GetPlayerClientByIndex(loginData.index);
	if (playerClient)
	{
		new(playerClient) PlayerClient(loginData.index);
	}
	else
	{
		playerClient = new PlayerClient(loginData.index);
	}

	playerClient->SetID(loginData.userId);
	playerClient->SetAnimalid(loginData.roleid);
	playerClient->SetRefreshTime(10);
	playerClient->SetLived(true);
	playerClient->SetAnimaltype((HeroType)loginData.roleType);
	playerClient->SetAnimalname(loginData.roleName);
	playerClient->SetPlayername(loginData.netName);

	m_PlayerClientVec[loginData.index] = playerClient;

	playerClient->LoadMysql();
	playerClient->CalAttrs();
	playerClient->EnterScene();
	playerClient->SetLoad(true);

	DTCPC->SendMsg(loginData.index, nullptr, 0, MsgCmd::MsgCmd_Login,
		(int)LoginSysMsgCmd::cs_login, 0, pInfo->bev, 0);

	return;
}
void PlayerCenter::HandlerPlayerThread()
{
	bool& run = DTCPC->GetRuninged();

	ListLoginData loadPlayerList;
	ListLoginData& playerList = m_LoadPlayerList;

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
	COUT_LOG(LOG_CINFO, "playerClient create thread end");
}