#include "../Game/stdafx.h"

PlayerCenter::PlayerCenter(Scene* pScene) : 
	m_pScene(pScene)
{
	CBaseCfgMgr& baseCfgMgr = CfgMgr()->GetCBaseCfgMgr();
	const LogicCfg& logicCfg = baseCfgMgr.GetLogicCfg();

	// 初始化分配内存
	unsigned int playerSize = logicCfg.maxSocketCnt * 2;
	m_pPlayerVec.resize((size_t)playerSize);

	// 玩家全部初始化为空
	for (auto* player : m_pPlayerVec)
	{
		player = nullptr;
	}

	std::vector<std::thread*>& threadVec = m_pScene->GetPlayerPreproces()->GetTCPClient()->GetSockeThreadVec();
	threadVec.push_back(new std::thread(&PlayerCenter::HandlerPlayerThread, this));
}

PlayerCenter::~PlayerCenter()
{

}

// 分发消息
void PlayerCenter::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{

}

// 玩家创建和数据库的加载
void PlayerCenter::HandlerPlayerThread()
{
	COUT_LOG(LOG_CINFO, "Scene::HandlerPlayerThread thread begin...");

	std::this_thread::sleep_for(std::chrono::seconds(1));

	while (m_pScene->GetPlayerPreproces()->GetTCPClient()->GetRuninged())
	{
		//进入挂起状态
		std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
		m_cond.Wait(uniqLock, [this] { if (this->m_LoadPlayerList.size() > 0) { return true; } return false; });

		if (this->m_LoadPlayerList.size() <= 0)
		{
			continue;
		}

		LoadPlayerList LoadPlayerList;
		std::swap(LoadPlayerList, m_LoadPlayerList);

		uniqLock.unlock();

		while (!LoadPlayerList.empty())
		{
			LoadPlayerKey loadPKey = LoadPlayerList.front();
			LoadPlayerList.pop_front();

			// 创建玩家
			if (!loadPKey.pSockInfo->isConnect)
			{
				continue;
			}

			Player* player = new Player(loadPKey.pSockInfo, loadPKey.userId);
			m_pPlayerVec[loadPKey.index] = player;

			player->LoadMysql();
			player->EnterGame();
			player->EnterScene();
		}
	}

	COUT_LOG(LOG_CINFO, "Scene::HandlerPlayerThread thread end...");
}


// 初始化回调函数
void PlayerCenter::InitCallBackFun()
{

}

// 创建角色
bool PlayerCenter::CreatePlayr(int index, const TCPSocketInfo* pSockInfo, std::string& userId)
{
	m_cond.GetMutex().lock();
	m_LoadPlayerList.push_back(LoadPlayerKey(index, pSockInfo, userId));
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();

	return true;
}

// 获取玩家
Player* PlayerCenter::GetPlayer(unsigned int index)
{
	return m_pPlayerVec[index];
}