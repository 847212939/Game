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

// �����Ϣ����
#include "../Player/MessageCmd.h"
// ����
#include "../kernel/Internal.h"
// ��־
#include "../kernel/Log.h"
// ���ݿ�
#include "../kernel/MysqlHelper.h"
// ��������
#include "../kernel/ConditionVariable.h"
// ���ݴ���
#include "../kernel/DataLine.h"
// �������
#include "../kernel/TCPSocket.h"
// ����ͻ���
#include "../kernel/TCPClient.h"
// ������
#include "../kernel/Util.h"
// ��־������
#include "../kernel/LogManager.h"

// ����
#include "../Player/Attrs.h"
// �����Ϣ
#include "../Player/PlayerInfo.h"
// ���
#include "../Player/Player.h"
// ��ҹ���
#include "../Player/PlayerCenter.h"
// ����
#include "../Player/Scene.h"
// ��Ϣ��ע��
#include "../Player/PlayerPreproces.h"


// SFIAE
#include "../study/Addition.h"
// �ɵ��ö���
#include "../study/Function.h"
// ��һ�������ظ�����һ������
#include "../study/IntegerSequence.h"


// �����õķ���������
#include "../study/Test.h"

// lua
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "tolua++.h"

// ���ù���
#include "ConfigMgr.h"


#pragma comment(lib,"ws2_32.lib")

