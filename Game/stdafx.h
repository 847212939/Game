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
#include <mysql.h>

// 网络
#include "../kernel/Internal.h"
// 日志
#include "../kernel/Log.h"
// 数据库
#include "../kernel/MysqlHelper.h"
// 条件变量
#include "../kernel/ConditionVariable.h"
// 数据处理
#include "../kernel/DataLine.h"
// 网络基类
#include "../kernel/TCPSocket.h"
// 网络客户端
#include "../kernel/TCPClient.h"
// 工具类
#include "../kernel/Util.h"
// 日志管理类
#include "../kernel/LogManager.h"

// 玩家消息定义
#include "../Player/MessageCmd.h"
// 玩家信息
#include "../Player/PlayerInfo.h"
// 玩家
#include "../Player/Player.h"
// 玩家管理
#include "../Player/PlayerCenter.h"
// 场景
#include "../Player/Scene.h"
// 信息在注册
#include "../Player/PlayerPreproces.h"
// 加密解密websocket
#include "../Player/Sha1Base64.h"
// websocket
#include "../Player/WebSocket.h"
// webserver
#include "../Player/server.h"
















// 模板学习
#include "../study/Mixin.h"

// 测试用的放在最下面
#include "../study/Test.h"

#pragma comment(lib,"ws2_32.lib")

