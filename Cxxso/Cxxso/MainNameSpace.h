#pragma once
#ifdef _WIN32
#else
namespace MainNameSpace
{
	extern "C"
	{
		// ���÷�����ip�˿�
		bool InitNetwork(char* ip, int port, int timerCnt);
		// ��ʼ�������
		bool InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc);
		// ����������Ϣ
		void SendData(char* pData, int size, int mainID, int assistID, int uIdentification);
		// ע�ᶨʱ��
		void RegisterTimers(int timerid, int uElapse);
		// ��ע�ᶨʱ��
		void UnRegisterTimers(int timerid);
	}
}
#endif