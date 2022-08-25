#pragma once

namespace MainNameSpace
{
	extern "C"
	{
		// 设置服务器ip端口
		EXPORT_DLL bool __stdcall InitNetwork(char* ip, int port, int timerCnt);
		// 初始化网络库
		EXPORT_DLL bool __stdcall InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc);
		// 发送网络消息
		EXPORT_DLL void __stdcall SendData(char* pData, int size, int mainID, int assistID, int uIdentification);
		// 注册定时器
		EXPORT_DLL void __stdcall RegisterTimers(int timerid, int uElapse);
		// 反注册定时器
		EXPORT_DLL void __stdcall UnRegisterTimers(int timerid);
	}
}