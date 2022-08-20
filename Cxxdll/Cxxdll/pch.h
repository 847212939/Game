#ifndef PCH_H
#define PCH_H

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// 添加要在此处预编译的标头
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

#include "Macro.h"
#include "MessageCmd.h"	
#include "Structural.h"

#include "ConditionVariable.h"
#include "DataLine.h"

#include "ServerTimer.h"
#include "TCPSocket.h"
#include "TCpClien.h"
#include "Util.h"

#include "BaseCfgMgr.h"	
#include "ConfigMgr.h"	
#include "LuaMgr.h"	
#include "MainNameSpace.h"

#endif //PCH_H
