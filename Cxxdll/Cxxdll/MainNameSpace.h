#pragma once
#include "Structural.h"

// g++ -shared -fpic -lm -ldl -levent -lpthread -std=c++11 -O3 -o Cxxdll.so *.cpp
namespace MainNameSpace
{
	extern "C"
	{
		// 设置服务器ip端口
		EXPORT_DLL bool InitNetwork(char* ip, int port, int timerCnt);
		// 初始化网络库
		EXPORT_DLL bool InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc);
		// 发送网络消息
		EXPORT_DLL bool SendData(char* pData, int size, int mainID, int assistID, int uIdentification);
		// 注册定时器
		EXPORT_DLL bool RegisterTimers(int timerid, int uElapse);
		// 反注册定时器
		EXPORT_DLL bool UnRegisterTimers(int timerid);
	}
}