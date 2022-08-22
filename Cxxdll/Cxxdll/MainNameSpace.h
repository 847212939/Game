#pragma once

namespace MainNameSpace
{
	extern "C"
	{
		// ���÷�����ip�˿�
		__declspec(dllexport) void __stdcall InitNetwork(char* ip, int port, int timerCnt);
		// ��ʼ�������
		__declspec(dllexport) bool __stdcall InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc);
		// ����������Ϣ
		__declspec(dllexport) void __stdcall SendData(char* pData, int size, int mainID, int assistID, int uIdentification);
		// ע�ᶨʱ��
		__declspec(dllexport) void __stdcall RegisterTimers(int timerid, int uElapse);
		// ��ע�ᶨʱ��
		__declspec(dllexport) void __stdcall UnRegisterTimers(int timerid);
	}
}