#pragma once

#define __DEBUG__

#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#elif defined(_WIN64)
#elif defined(__linux__)
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<string.h>
#elif defined(__unix__)
#elif defined(__ANDROID__)
#elif defined(__APPLE__)
#endif

#include <iomanip>
#include <random>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>
#include <array>
#include <atomic>
#include <chrono>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <functional>
#include <condition_variable>
#include <time.h>
#include "mysql.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/listener.h"
#include "event2/util.h"
#include "event2/event.h"
#include "event2/thread.h"
#include "event2/event_struct.h"
#include "event2/bufferevent_ssl.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/x509.h"
#include "openssl/x509v3.h"

// lua
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "tolua++.h"

#include "kernel/Macro.h"				// 宏
#include "kernel/MessageCmd.h"			// 玩家消息定义
#include "kernel/AttrsCmd.h"			// 玩家属性
#include "kernel/TimerCmd.h"			// 定时器
#include "kernel/Structural.h"			// 结构
#include "kernel/Alias.h"				// 别名
#include "kernel/MysqlHelper.h"			// 数据库
#include "kernel/ConditionVariable.h"	// 条件变量
#include "kernel/DataLine.h"			// 数据处理
#include "kernel/TCPSocket.h"			// 网络基类
#include "kernel/TCPClient.h"			// 网络客户端
#include "kernel/Util.h"				// 工具类
#include "kernel/Netmsg.h"				// 消息解析
#include "kernel/LogManager.h"			// 日志管理类
#include "kernel/ServerTimer.h"			// 定时器

#include "Player/PlayerPrep.h"			// 信息在注册
#include "Player/PlayerPrepClient.h"	// 逻辑处理类

#include "Lua/BaseCfgMgr.h"				// 网络配置
#include "Lua/ConfigMgr.h"				// 配置管理
#include "Lua/LuaMgr.h"					// lua虚拟机创建
