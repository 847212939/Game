#include "../stdafx.h"

PlayerCenter::PlayerCenter()
{
	
}
PlayerCenter::~PlayerCenter()
{

}
void PlayerCenter::Init()
{
	int maxSocketCnt = G_CfgMgr->GetCBaseCfgMgr().GetMaxSocketCnt();

	// 初始化分配内存
	unsigned int playerSize = maxSocketCnt * 2;
	m_PlayerClientVec.resize((size_t)playerSize);

	// 玩家全部初始化为空
	for (auto* playerClient : m_PlayerClientVec)
	{
		playerClient = nullptr;
	}
	
	G_NetClient->GetSockeThreadVec().push_back(new std::thread(&PlayerCenter::HandlerPlayerThread, this));
}

// 创建角色
void PlayerCenter::CreatePlayer(LoginData& loginData)
{
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_LoadPlayerList.push_back(loginData);
	}

	m_cond.notify_one();
}
VectorPlayerClient& PlayerCenter::GetVectorPlayerClient()
{
	return m_PlayerClientVec;
}
// 获取玩家
PlayerClient* PlayerCenter::GetPlayerByIndex(unsigned int index)
{
	if (index >= m_PlayerClientVec.size() || index < 0)
	{
		return nullptr;
	}
	return m_PlayerClientVec[index];
}
PlayerClient* PlayerCenter::GetPlayerLogicByUserid(uint64_t& userId)
{
	std::vector<unsigned int> playerClientSet;
	G_NetClient->GetSocketSet(playerClientSet);
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
PlayerClient* PlayerCenter::GetPlayerCrossByUserid(uint64_t& userId)
{
	MapPlayerClient::iterator it = m_MapPlayerClient.find(userId);
	if (it == m_MapPlayerClient.end())
	{
		return nullptr;
	}
	return it->second;
}
PlayerClient* PlayerCenter::GetPlayerByUserid(uint64_t userId)
{
	return G_NetClient->GetServerType() ==
		ServiceType::SERVICE_TYPE_CROSS ?
		GetPlayerCrossByUserid(userId) :
		GetPlayerLogicByUserid(userId);
}
void PlayerCenter::GetSocketSet(std::vector<unsigned int>& socketVec)
{
	// 获取在线玩家
	G_NetClient->GetSocketSet(socketVec);
}
std::condition_variable& PlayerCenter::GetConditionVariable()
{
	return m_cond;
}

void PlayerCenter::HandleLogicLoadPlayer(LoginData& loginData)
{
	const TCPSocketInfo* pInfo = G_NetClient->GetTCPSocketInfo(loginData.index);
	if (!pInfo)
	{
		Log(CERR, "Client information is empty index=%d", loginData.index);
		return;
	}
	if (loginData.index < 0 || loginData.index >= m_PlayerClientVec.size())
	{
		Log(CINF, "loginData.index < 0 || loginData.index >= m_PlayerClientVec.size()");
		G_NetClient->CloseSocket(loginData.index);
		return;
	}
	if (!pInfo->isConnect)
	{
		Log(CINF, "!pInfo->isConnect");
		G_NetClient->CloseSocket(loginData.index);
		return;
	}
	PlayerClient* playerClient = GetPlayerByIndex(loginData.index);
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

	G_NetClient->SendMsg(loginData.index, nullptr, 0, MsgCmd::MsgCmd_Login,
		(int)LoginSysMsgCmd::cs_login, 0, pInfo->bev, 0, loginData.userId);

	return;
}
void PlayerCenter::HandleCrossLoadPlayer(LoginData& loginData)
{
	const TCPSocketInfo* pLogicServerTcpInfo = G_NetClient->GetTCPSocketInfo(loginData.index);
	if (!pLogicServerTcpInfo)
	{
		Log(CERR, "!pLogicServerTcpInfo logicServerindex=%d", loginData.index);
		return;
	}
	if (!pLogicServerTcpInfo->isConnect)
	{
		Log(CINF, "!pInfo->isConnect logicServerindex=%d", loginData.index);
		G_NetClient->CloseSocket(loginData.index);
		return;
	}
	PlayerClient* playerClient = GetPlayerByUserid(loginData.userId);
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
	playerClient->SetLogicIndex(loginData.logicIndex);
	playerClient->SetServerid(loginData.serverId);

	AddMapPlayerClient(loginData.userId, playerClient);

	playerClient->LoadMysql();
	playerClient->CalAttrs();
	playerClient->EnterScene();
	playerClient->SetLoad(true);

	return;
}
void PlayerCenter::HandleLoadPlayer(LoginData& loginData)
{
	G_NetClient->GetServerType() == 
		ServiceType::SERVICE_TYPE_CROSS ? 
		HandleCrossLoadPlayer(loginData) : 
		HandleLogicLoadPlayer(loginData);
}
void PlayerCenter::HandlerPlayerThread()
{
	bool& run = G_NetClient->GetRuninged();
	while (run)
	{
		std::unique_lock<std::mutex> uniqLock(m_mutex);
		m_cond.wait(uniqLock, [this, &run] {return this->m_LoadPlayerList.size() > 0 || !run; });
		if (m_LoadPlayerList.size() <= 0)
		{
			uniqLock.unlock();
			continue;
		}

		LoginData data = m_LoadPlayerList.front();
		m_LoadPlayerList.pop_front();
		uniqLock.unlock();

		HandleLoadPlayer(data);
	}
	Log(CINF, "playerClient create thread end");
}

void PlayerCenter::AddMapPlayerClient(uint64_t& userid, PlayerClient* player)
{
	auto it = m_MapPlayerClient.find(userid);
	if (it != m_MapPlayerClient.end())
	{
		return;
	}
	m_MapPlayerClient.insert({ userid , player });
}
void PlayerCenter::DelMapPlayerClient(uint64_t& userid)
{
	auto it = m_MapPlayerClient.find(userid);
	if (it == m_MapPlayerClient.end())
	{
		return;
	}
	SafeDelete(it->second);
	m_MapPlayerClient.erase(it);
}