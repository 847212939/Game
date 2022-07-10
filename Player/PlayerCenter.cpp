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
	SubPlayerPreproces* pSubPlayerPreproces = m_pScene->GetPlayerPreproces();
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
	const LogicCfg& logicCfg = baseCfgMgr.GetLogicCfg();

	// 初始化分配内存
	unsigned int playerSize = logicCfg.maxSocketCnt * 2;
	m_pPlayerVec.resize((size_t)playerSize);

	// 玩家全部初始化为空
	for (auto* pSubPlayer : m_pPlayerVec)
	{
		pSubPlayer = nullptr;
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
	if (!pSubPlayer->GetLoad())
	{
		COUT_LOG(LOG_CERROR, "Dispatch message mysql is unload index = %u", pPlayerInfo->m_pMsg->uIndex);
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
	if (!pTCPClient->GetRuninged())
	{
		COUT_LOG(LOG_CERROR, "PlayerCenter::HandlerPlayerThread 初始化未完成");
		return;
	}

	while (pTCPClient->GetRuninged())
	{
		//进入挂起状态
		std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
		m_cond.Wait(uniqLock, [this] { if (this->m_LoadPlayerList.size() > 0) { return true; } return false; });

		if (this->m_LoadPlayerList.size() <= 0)
		{
			uniqLock.unlock();
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
			uint64_t userId = 0;
			if (!pSubPlayerPreproces->GetLoginSys().LoginIn(loadPKey.id, loadPKey.pw, userId))
			{
				continue;
			}
			if (userId == 0)
			{
				continue;
			}
			pTCPClient->SendData(loadPKey.GetIndex(), NULL, 0, MsgCmd::MsgCmd_Login, 1, 0, loadPKey.GetSocketInfo()->bev);
			SubPlayer* pSubPlayer = GetSubPlayer(loadPKey.GetIndex());
			if (pSubPlayer)
			{
				new(pSubPlayer) SubPlayer(loadPKey.GetIndex(), loadPKey.GetSocketInfo(), userId, pSubPlayerPreproces);
			}
			else
			{
				pSubPlayer = new SubPlayer(loadPKey.GetIndex(), loadPKey.GetSocketInfo(), userId, pSubPlayerPreproces);
			}
			m_pPlayerVec[loadPKey.GetIndex()] = pSubPlayer;

			pSubPlayer->LoadMysql();
			pSubPlayer->EnterGame();
			pSubPlayer->EnterScene();
			pSubPlayer->SetLoad(true);
		}
	}

	COUT_LOG(LOG_CINFO, "pSubPlayer create thread end...");
}

void PlayerCenter::SetSubScene(SubScene* pSubScene)
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
SubPlayer* PlayerCenter::GetSubPlayer(unsigned int index)
{
	return m_pPlayerVec[index];
}

// 获取场景
const SubScene* PlayerCenter::getScene()
{
	return m_pScene;
}
