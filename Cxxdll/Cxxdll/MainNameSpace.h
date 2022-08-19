#pragma once

namespace MainNameSpace
{
	extern "C"
	{
		__declspec(dllexport) void __stdcall RegisterCallBack(void (*pfCallBackEvent)(REvent eve/*, char m_Source[]*/));
		__declspec(dllexport) void __stdcall InitCxxnet();
		__declspec(dllexport) void __stdcall SendData(char* pData, int size, int mainID, int assistID, int uIdentification);
	}
}