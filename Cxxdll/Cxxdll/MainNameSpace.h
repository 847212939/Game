#pragma once

namespace MainNameSpace
{
	extern "C"
	{
		// ���÷�����ip�˿�
		EXPORT_DLL bool __stdcall InitNetwork(char* ip, int port, int timerCnt);
		// ��ʼ�������
		EXPORT_DLL bool __stdcall InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc);
		// ����������Ϣ
		EXPORT_DLL void __stdcall SendData(char* pData, int size, int mainID, int assistID, int uIdentification);
		// ע�ᶨʱ��
		EXPORT_DLL void __stdcall RegisterTimers(int timerid, int uElapse);
		// ��ע�ᶨʱ��
		EXPORT_DLL void __stdcall UnRegisterTimers(int timerid);
	}
}