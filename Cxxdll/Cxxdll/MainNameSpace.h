#pragma once
#include "Structural.h"

// g++ -shared -fpic -lm -ldl -levent -lpthread -std=c++11 -O3 -o Cxxdll.so *.cpp
namespace MainNameSpace
{
	extern "C"
	{
		// 设置服务器ip端口
		EXPORT_DLL int EXPORT_STDCALL InitNetwork(char* ip, int port, int timerCnt, char* key);
		// 初始化网络库
		EXPORT_DLL int EXPORT_STDCALL InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc, CloseCallBackFunc closeFunc);
		// 反初始化网络库
		EXPORT_DLL int EXPORT_STDCALL UnInitCxxnet();
		// 发送网络消息
		EXPORT_DLL int EXPORT_STDCALL SendData(char* pData, int size, int mainID, int assistID, int uIdentification);
		// 注册定时器
		EXPORT_DLL int EXPORT_STDCALL RegisterTimers(int timerid, int uElapse);
		// 反注册定时器
		EXPORT_DLL int EXPORT_STDCALL UnRegisterTimers(int timerid);
	}
}