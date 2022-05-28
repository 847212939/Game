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

// �����Ϣ����
#include "../Player/MessageCmd.h"
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
// ���ܽ���websocket
#include "../Player/Sha1Base64.h"
// websocket
#include "../Player/WebSocket.h"
// webserver
#include "../Player/server.h"
















// ģ��ѧϰ
#include "../study/Mixin.h"

// �����õķ���������
#include "../study/Test.h"

#pragma comment(lib,"ws2_32.lib")

