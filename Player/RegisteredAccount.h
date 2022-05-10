#pragma once
#include "PlayerInclude.h"
#include "PlayerCenter.h"

namespace PLAYER
{
	struct PlayerInfo;
	class RegisteredAccount
	{
		// 注册账号类
	public:
		RegisteredAccount();
		virtual ~RegisteredAccount();

	public:
		// 启动数据库
		bool StartDB();
		//	处理消息
		void HandelMessage(PlayerInfo* pInfo);
		// 分发消息
		void DispatchMessage();
		// 创建角色
		bool CreatePlayr();

	private:
		// 玩家中心管理器
		PlayerCenter m_PlayerCenter;
		// 数据库
		CMysqlHelper m_CMysqlHelper;
	};
}

