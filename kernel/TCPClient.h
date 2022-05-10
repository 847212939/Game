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
		// 收发数据的循环处理线程入口
		void Run();
		// 服务器收到客户端的消息队列
		void HandlerRecvDataList();

	private:
		// 注册信息检查
		PLAYER::RegisteredAccount m_RegisteredAccount;
	};
}
