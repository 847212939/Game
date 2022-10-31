#pragma once

// g++ -shared -fpic -lm -ldl -levent -lpthread -std=c++11 -O3 -o Cxxdll.so *.cpp
namespace MainNameSpace
{
	extern "C"
	{
		// ���÷�����ip�˿�
		EXPORT_DLL int EXPORT_STDCALL InitNetwork(char* ip, int port, int timerCnt);
		// ��ʼ�������
		EXPORT_DLL int EXPORT_STDCALL InitCxxnet(
			NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc, CloseCallBackFunc closeFunc);
		// ����ʼ�������
		EXPORT_DLL int EXPORT_STDCALL UnInitCxxnet();
		// ����������Ϣ
		EXPORT_DLL int EXPORT_STDCALL SendMsg(char* pData, int size, int mainID, int assistID, int uIdentification);
		// ע�ᶨʱ��
		EXPORT_DLL int EXPORT_STDCALL RegisterTimers(int timerid, int uElapse);
		// ��ע�ᶨʱ��
		EXPORT_DLL int EXPORT_STDCALL UnRegisterTimers(int timerid);
	}
}