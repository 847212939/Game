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

#include "kernel/Macro.h"				// ��
#include "kernel/MessageCmd.h"			// �����Ϣ����
#include "kernel/AttrsCmd.h"			// �������
#include "kernel/TimerCmd.h"			// ��ʱ��
#include "kernel/Structural.h"			// �ṹ
#include "kernel/Alias.h"				// ����
#include "kernel/MysqlHelper.h"			// ���ݿ�
#include "kernel/ConditionVariable.h"	// ��������
#include "kernel/DataLine.h"			// ���ݴ���
#include "kernel/TCPSocket.h"			// �������
#include "kernel/TCPClient.h"			// ����ͻ���
#include "kernel/Util.h"				// ������
#include "kernel/Netmsg.h"				// ��Ϣ����
#include "kernel/LogManager.h"			// ��־������
#include "kernel/ServerTimer.h"			// ��ʱ��

#include "Player/PlayerPrep.h"			// ��Ϣ��ע��
#include "Player/PlayerPrepClient.h"	// �߼�������

#include "Lua/BaseCfgMgr.h"				// ��������
#include "Lua/ConfigMgr.h"				// ���ù���
#include "Lua/LuaMgr.h"					// lua���������
