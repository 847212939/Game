#pragma once
#include "Structural.h"

// g++ -shared -fpic -lm -ldl -levent -lpthread -std=c++11 -O3 -o Cxxdll.so *.cpp
namespace MainNameSpace
{
	extern "C"
	{
		// ���÷�����ip�˿�
		EXPORT_DLL bool InitNetwork(char* ip, int port, int timerCnt);
		// ��ʼ�������
		EXPORT_DLL bool InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc);
		// ����������Ϣ
		EXPORT_DLL bool SendData(char* pData, int size, int mainID, int assistID, int uIdentification);
		// ע�ᶨʱ��
		EXPORT_DLL bool RegisterTimers(int timerid, int uElapse);
		// ��ע�ᶨʱ��
		EXPORT_DLL bool UnRegisterTimers(int timerid);
	}
}