#pragma once

// ���Ҫ�ڴ˴�Ԥ����ı�ͷ
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
#include "Macro.h"				// ��
#include "MessageCmd.h"			// �����Ϣ����
#include "Structural.h"			// �ṹ
#include "Alias.h"				// ����
#include "MysqlHelper.h"		// ���ݿ�
#include "DataLine.h"			// ���ݴ���
#include "TCPSocket.h"			// �������
#include "TCPClient.h"			// ����ͻ���
#include "Util.h"				// ������
#include "Netmsg.h"				// ��Ϣ����
#include "LogManager.h"			// ��־������

#include "DataBaseSys.h"		// ���ݿ����
#include "PlayerPrep.h"			// ��Ϣ��ע��
#include "PlayerPrepClient.h"	// �߼�������

#include "BaseCfgMgr.h"				// ��������
#include "ConfigMgr.h"				// ���ù���
#include "LuaMgr.h"					// lua���������
#include "mainspace.h"
