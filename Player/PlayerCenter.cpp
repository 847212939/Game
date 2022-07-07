#include "../Game/stdafx.h"

PlayerCenter::PlayerCenter(SubScene* pSubScene) : 
	m_pScene(pSubScene)
{
	CBaseCfgMgr& baseCfgMgr = CfgMgr()->GetCBaseCfgMgr();
	const LogicCfg& logicCfg = baseCfgMgr.GetLogicCfg();

	// 初始化分配内存
	unsigned int playerSize = logicCfg.maxSocketCnt * 2;
	m_pPlayerVec.resize((size_t)playerSize);

	// 玩家全部初始化为空
	for (auto* pSubPlayer : m_pPlayerVec)
	{
		pSubPlayer = nullptr;
	}

	if (m_pScene)
	{
		SubPlayerPreproces* pSubPlayerPreproces = m_pScene->GetPlayerPreproces();
		if (pSubPlayerPreproces)
		{
			TCPClient* pTCPClient = pSubPlayerPreproces->GetTCPClient();
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

// 分发消息
void PlayerCenter::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	if (!m_pScene)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message scent = null cmd = %d", cmd);
		return;
	}
	SubPlayerPreproces* pSubPlayerPreproces = m_pScene->GetPlayerPreproces();
	if (!pSubPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message player preproces = null cmd = %d", cmd);
		return;
	}
	if (!pPlayerInfo)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message pSubPlayer info = null cmd = %d", cmd);
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
	SubPlayer* pSubPlayer = GetSubPlayer(pPlayerInfo->m_pMsg->uIndex);
	if (!pSubPlayer)
	{
		COUT_LOG(LOG_CERROR, "Dispatch message pSubPlayer = null index = %u", pPlayerInfo->m_pMsg->uIndex);
		return;
	}
	if (strcmp(pSubPlayer->GetTCPSocketInfo()->ip, pPlayerInfo->m_pTcpSockInfo->ip) != 0)
	{
		COUT_LOG(LOG_CERROR, "The local IP address and remote IP address are not equal");
		return;
	}
	if (pSubPlayer->GetIndex() != pPlayerInfo->m_pMsg->uIndex)
	{
		COUT_LOG(LOG_CERROR, "The socket index received by the pSubPlayer is inconsistent "
			"pSubPlayer->GetIndex() = %u, "
			"pPlayerInfo->m_pMsg->uIndex = %u", 
			pSubPlayer->GetIndex(), 
			pPlayerInfo->m_pMsg->uIndex);
		return;
	}

	switch (cmd)
	{
	case MsgCmd::MsgCmd_PlayerCenter:
		// 玩家中心处理的消息.. 玩家间的互动
		pSubPlayerPreproces->CallBackFun(cmd, pPlayerInfo);
		break;
	default:
		pSubPlayer->DispatchMessage(cmd, pPlayerInfo);
		break;
	}
}

// 玩家创建和数据库的加载
void PlayerCenter::HandlerPlayerThread()
{
	COUT_LOG(LOG_CINFO, "pSubPlayer create thread begin...");

	if (!m_pScene)
	{
		COUT_LOG(LOG_CERROR, "pSubPlayer create thread err m_pScene = null");
		return;
	}
	SubPlayerPreproces* pSubPlayerPreproces = m_pScene->GetPlayerPreproces();
	if (!pSubPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "pSubPlayer create thread err pSubPlayerPreproces = null");
		return;
	}
	TCPClient* pTCPClient = pSubPlayerPreproces->GetTCPClient();
	if (!pTCPClient)
	{
		COUT_LOG(LOG_CERROR, "pSubPlayer create thread err pTCPClient = null");
		return;
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));

	while (pTCPClient->GetRuninged())
	{
		//进入挂起状态
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

			if (!loadPKey.GetConnect())
			{
				continue;
			}
			SubPlayer* pSubPlayer = GetSubPlayer(loadPKey.GetIndex());
			if (pSubPlayer)
			{
				new(pSubPlayer) SubPlayer(loadPKey.GetIndex(), loadPKey.GetSocketInfo(), loadPKey.getUserId());
			}
			else
			{
				pSubPlayer = new SubPlayer(loadPKey.GetIndex(), loadPKey.GetSocketInfo(), loadPKey.getUserId());
			}

			pSubPlayer->SetPlayerPreproces(pSubPlayerPreproces);

			m_pPlayerVec[loadPKey.GetIndex()] = pSubPlayer;

			pSubPlayer->CallBackFunInit();
			pSubPlayer->LoadMysql();
			pSubPlayer->EnterGame();
			pSubPlayer->EnterScene();
		}
	}

	COUT_LOG(LOG_CINFO, "pSubPlayer create thread end...");
}

// 创建角色
bool PlayerCenter::CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId)
{
	m_cond.GetMutex().lock();
	m_LoadPlayerList.push_back(LoadPlayerKey(index, pSockInfo, userId));
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();

	return true;
}

// 获取玩家
SubPlayer* PlayerCenter::GetSubPlayer(unsigned int index)
{
	return m_pPlayerVec[index];
}

// 获取场景
const SubScene* PlayerCenter::getScene()
{
	return m_pScene;
}
