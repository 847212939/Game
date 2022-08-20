#pragma once

namespace MainNameSpace
{
	extern "C"
	{
		// ��ʼ�������
		__declspec(dllexport) void __stdcall InitCxxnet(NetworkCallBackFunc func);
		// ����������Ϣ
		__declspec(dllexport) void __stdcall SendData(char* pData, int size, int mainID, int assistID, int uIdentification);
		// ע�ᶨʱ��
		__declspec(dllexport) void __stdcall RegisterTimers(int timerid, int uElapse, pCallBackTimer func);
		__declspec(dllexport) void __stdcall UnRegisterTimers(int timerid);
	}
}