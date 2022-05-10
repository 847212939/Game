#pragma once
#include "Internal.h"
#include "DataLine.h"
#include "TCPSocket.h"
#include "KernelInclude.h"

namespace DRMU
{
	class RegisteredAccount;
	class TCPClient : public CTCPSocketManage
	{
	public:
		TCPClient();
		virtual ~TCPClient();

	private:
		// �շ����ݵ�ѭ�������߳����
		void Run();
		// �������յ��ͻ��˵���Ϣ����
		void HandlerRecvDataList();

	private:
		// ע����Ϣ���
		PLAYER::RegisteredAccount m_RegisteredAccount;
	};
}
