#pragma once

namespace MainNameSpace
{
	extern "C"
	{
		// 初始化网络库
		__declspec(dllexport) void __stdcall InitCxxnet(NetworkCallBackFunc func);
		// 发送网络消息
		__declspec(dllexport) void __stdcall SendData(char* pData, int size, int mainID, int assistID, int uIdentification);
		// 注册定时器
		__declspec(dllexport) void __stdcall RegisterTimers(int timerid, int uElapse, pCallBackTimer func);
		__declspec(dllexport) void __stdcall UnRegisterTimers(int timerid);
	}
}