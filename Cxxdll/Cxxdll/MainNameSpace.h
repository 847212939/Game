#pragma once

namespace MainNameSpace
{
	extern "C"
	{
		__declspec(dllexport) void RegisterCallBack(void (*pfCallBackEvent)(REvent eve/*, char m_Source[]*/));
		__declspec(dllexport) void __stdcall InitCxxnet();
	}
}