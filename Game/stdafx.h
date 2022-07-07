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

#include "../Player/MessageCmd.h"			// �����Ϣ����
#include "../kernel/Internal.h"				// ����
#include "../kernel/Log.h"					// ��־
#include "../kernel/MysqlHelper.h"			// ���ݿ�
#include "../kernel/ConditionVariable.h"	// ��������
#include "../kernel/DataLine.h"				// ���ݴ���
#include "../kernel/TCPSocket.h"			// �������
#include "../kernel/TCPClient.h"			// ����ͻ���
#include "../kernel/Util.h"					// ������
#include "../kernel/LogManager.h"			// ��־������

#include "../Player/Attribute.h"			// ����
#include "../Player/PlayerInfo.h"			// �����Ϣ
#include "../Player/Player.h"				// ���
#include "../SubSys/SubPlayer.h"			// �߼�������
#include "../Player/PlayerCenter.h"			// ��ҹ���
#include "../SubSys/SubPlayerCenter.h"		// ��ҹ���
#include "../Player/Scene.h"				// ����
#include "../SubSys/SubScene.h"				// ����
#include "../Player/PlayerPreproces.h"		// ��Ϣ��ע��
#include "../SubSys/SubPlayerPreproces.h"			// �߼�������

#include "../study/Addition.h"				// SFIAE
#include "../study/Function.h"				// �ɵ��ö���
#include "../study/IntegerSequence.h"		// ��һ�������ظ�����һ������
#include "../study/Test.h"					// �����õķ���������

#include "../Config/BaseCfgMgr.h"			// ��������
#include "../Config/ConfigMgr.h"			// ���ù���
#include "../Lua/LuaMgr.h"					// lua���������
