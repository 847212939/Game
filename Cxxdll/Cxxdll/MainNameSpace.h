#pragma once
#include "Structural.h"

namespace MainNameSpace
{
	extern "C"
	{
		// ���÷�����ip�˿�
		EXPORT_DLL bool InitNetwork(char* ip, int port, int timerCnt);
		// ��ʼ�������
		EXPORT_DLL bool InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc);
		// ����������Ϣ
		EXPORT_DLL void SendData(char* pData, int size, int mainID, int assistID, int uIdentification);
		// ע�ᶨʱ��
		EXPORT_DLL void RegisterTimers(int timerid, int uElapse);
		// ��ע�ᶨʱ��
		EXPORT_DLL void UnRegisterTimers(int timerid);
	}
}