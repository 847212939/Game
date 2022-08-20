#pragma once

namespace MainNameSpace
{
	extern "C"
	{
		// ���÷�����ip�˿�
		__declspec(dllexport) void __stdcall SetIpAndPort(char* ip, int port);
		// ��ʼ�������
		__declspec(dllexport) void __stdcall InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc);
		// ����������Ϣ
		__declspec(dllexport) void __stdcall SendData(char* pData, int size, int mainID, int assistID, int uIdentification);
		// ע�ᶨʱ��
		__declspec(dllexport) void __stdcall RegisterTimers(int timerid, int uElapse);
		__declspec(dllexport) void __stdcall UnRegisterTimers(int timerid);
	}
}