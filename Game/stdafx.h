#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
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
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>
#include <mysql.h>

// lua
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "tolua++.h"

#include "../Player/MessageCmd.h"			// 玩家消息定义
#include "../kernel/Internal.h"				// 网络
#include "../kernel/Log.h"					// 日志
#include "../kernel/MysqlHelper.h"			// 数据库
#include "../kernel/ConditionVariable.h"	// 条件变量
#include "../kernel/DataLine.h"				// 数据处理
#include "../kernel/TCPSocket.h"			// 网络基类
#include "../kernel/TCPClient.h"			// 网络客户端
#include "../kernel/Util.h"					// 工具类
#include "../kernel/LogManager.h"			// 日志管理类

#include "../Player/Attribute.h"			// 属性
#include "../Player/PlayerInfo.h"			// 玩家信息
#include "../Player/Player.h"				// 玩家
#include "../SubSys/SubPlayer.h"			// 逻辑处理类
#include "../Player/PlayerCenter.h"			// 玩家管理
#include "../SubSys/SubPlayerCenter.h"		// 玩家管理
#include "../Player/Scene.h"				// 场景
#include "../SubSys/SubScene.h"				// 场景
#include "../Player/PlayerPreproces.h"		// 信息在注册
#include "../SubSys/SubPlayerPreproces.h"			// 逻辑处理类

#include "../study/Addition.h"				// SFIAE
#include "../study/Function.h"				// 可调用对象
#include "../study/IntegerSequence.h"		// 将一个数字重复生成一个类型
#include "../study/Test.h"					// 测试用的放在最下面

#include "../Config/BaseCfgMgr.h"			// 网络配置
#include "../Config/ConfigMgr.h"			// 配置管理
#include "../Lua/LuaMgr.h"					// lua虚拟机创建
