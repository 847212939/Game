#pragma once

#define __DEBUG__

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
#include <event2/event_struct.h>
#include <mysql.h>
#include <time.h>

// lua
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "tolua++.h"

#include "../kernel/Macro.h"				// 宏
#include "../Player/MessageCmd.h"			// 玩家消息定义
#include "../Player/AttrsCmd.h"				// 玩家属性
#include "../Player/TimerCmd.h"				// 定时器
#include "../kernel/Structural.h"			// 结构
#include "../Player/Attribute.h"			// 属性
#include "../kernel/MysqlHelper.h"			// 数据库
#include "../kernel/ConditionVariable.h"	// 条件变量
#include "../kernel/DataLine.h"				// 数据处理
#include "../kernel/TCPSocket.h"			// 网络基类
#include "../kernel/TCPClient.h"			// 网络客户端
#include "../kernel/Util.h"					// 工具类
#include "../kernel/LogManager.h"			// 日志管理类

#include "../Logic/MoveSys.h"				// 移动
#include "../Player/Player.h"				// 玩家
#include "../Player/PlayerClient.h"			// 逻辑处理类

#include "../Player/PlayerCenter.h"			// 玩家管理
#include "../Player/PlayerCenterClient.h"	// 玩家管理

#include "../Logic/Monster.h"				// 怪物系统
#include "../Logic/MonsterClient.h"			// 怪物系统
#include "../Logic/ActivityHallCfg.h"		// 活动配置
#include "../Logic/ActiveTime.h"			// 固定时间活动
#include "../Logic/ActiveService.h"			// 开服活动
#include "../Logic/ActiveAlways.h"			// 全天活动
#include "../Logic/ActiveSection.h"			// 时间内活动
#include "../Logic/ActivityHallSys.h"		// 活动系统
#include "../Player/Scene.h"				// 场景
#include "../Player/SceneClient.h"			// 场景
#include "../Player/ServerTimer.h"			// 定时器

#include "../Logic/LoginSys.h"				// 登录系统
#include "../Player/PlayerPrep.h"			// 信息在注册
#include "../Player/PlayerPrepClient.h"		// 逻辑处理类

#include "../Lua/BaseCfgMgr.h"				// 网络配置
#include "../Lua/ConfigMgr.h"				// 配置管理
#include "../Lua/LuaMgr.h"					// lua虚拟机创建
