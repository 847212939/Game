#pragma once
#include "PlayerInclude.h"
#include "PlayerCenter.h"

namespace PLAYER
{
	struct PlayerInfo;
	class RegisteredAccount
	{
		// ע���˺���
	public:
		RegisteredAccount();
		virtual ~RegisteredAccount();

	public:
		// �������ݿ�
		bool StartDB();
		//	������Ϣ
		void HandelMessage(PlayerInfo* pInfo);
		// �ַ���Ϣ
		void DispatchMessage();
		// ������ɫ
		bool CreatePlayr();

	private:
		// ������Ĺ�����
		PlayerCenter m_PlayerCenter;
		// ���ݿ�
		CMysqlHelper m_CMysqlHelper;
	};
}

