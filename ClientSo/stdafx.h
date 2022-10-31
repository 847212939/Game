#pragma once

// 添加要在此处预编译的标头
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

#include "framework.h"
#include "Macro.h"				// 宏
#include "MessageCmd.h"			// 玩家消息定义
#include "Structural.h"			// 结构
#include "Alias.h"				// 别名
#include "MysqlHelper.h"		// 数据库
#include "DataLine.h"			// 数据处理
#include "TCPSocket.h"			// 网络基类
#include "TCPClient.h"			// 网络客户端
#include "Util.h"				// 工具类
#include "Netmsg.h"				// 消息解析
#include "LogManager.h"			// 日志管理类

#include "DataBaseSys.h"		// 数据库操作
#include "PlayerPrep.h"			// 信息在注册
#include "PlayerPrepClient.h"	// 逻辑处理类

#include "BaseCfgMgr.h"				// 网络配置
#include "ConfigMgr.h"				// 配置管理
#include "LuaMgr.h"					// lua虚拟机创建
#include "mainspace.h"
