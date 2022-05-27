#include "stdafx.h"

int main()
{
	std::vector<int> a;
	std::vector<int> b;

	std::vector<int> c = a + b;

	return 0;
}

//int main()
//{
//	TestFun();
//	LogManager()->SetLogFileType(ServiceType::SERVICE_TYPE_LOGIC);
//	TCPClient clien;
//}

//#include <set>
//
//// 活动内玩家信息
//struct Data
//{
//	int playerId = 0;
//	int sorce = 0;
//	int killPeopleCount = 0;
//
//	bool operator < (const Data& other) const
//	{
//		if (sorce != other.sorce) return sorce < other.sorce;
//		return false;
//	}
//
//	Data(int s) { sorce = s; }
//	Data() {}
//	~Data() {}
//};
//
//typedef	std::multiset<Data> RankSet;
//
//int main()
//{
//	/*LogManager()->SetLogFileType(ServiceType::SERVICE_TYPE_LOGIC);
//	TCPClient clien;*/
//
//	RankSet mySet;
//	Data data;
//	data.sorce = 100;
//	mySet.insert(data);
//
//	mySet.insert(data);
//	mySet.insert(data);
//	mySet.insert(data);
//
//	Data data1;
//	data1.sorce = 500;
//	mySet.insert(data1);
//
//	Data data2;
//	data2.sorce = 20;
//	mySet.insert(data2);
//
//	Data data3;
//	data3.sorce = 200;
//	mySet.insert(data3);
//
//	const int x = 100;
//	size_t count = mySet.count(x);
//
//	return 0;
//}

//int main()
//{
//	//std::function<void(int,int)> func = std::bind(&Subscriber::Notify, this, std::placeholders::_1, std::placeholders::_2);
//	std::function<bool(PlayerInfo*)> f;
//	return 0;
//}
//
//

// 
// 
// 
//
//#include "../kernel/Util.h"
//#include <iostream>
//#include <sstream>
//
//
//using namespace DRMU;
//
//int main()
//{
//	std::string str = Util::CreateUuid();
//	std::cout << str << std::endl;
//	
//	COstringstream os;
//
//	os << 1005;
//	os << 1009238232;
//	os << 233;
//	os << 700;
//	os << 888;
//	std::string sss = "s hds  ssdwcce sv chw cs c c c c c c c c c c c c  c c c  c c c c  c c c ww w w w w  w w w  w w w w w w  w s sssssssssssssssssssssssssssssss s s s s sssss                  ";
//	os << sss;
//	char buf[] = "sdggggggggggggggggggsb sshiw w rrrrrrrrrrs aaaaaaaad w          sssss                    d";
//	os << buf;
//	os << "iiiiii";
//
//	CIstringstream is(os);
//	int a = 0;
//	is >> a;
//	long long b = 0;
//	is >> b;
//	int c = 0;
//	is >> c;
//	int d = 0;
//	is >> d;
//	int e = 0;
//	is >> e;
//	std::string str1;
//	is >> str1;
//	std::string s;
//	is >> s;
//	std::string ss;
//	is >> ss;
//
//	
//
//
//	return 0;
//}

//
//std::vector<std::string> mVec;
//
//template<class T>
//void test(T t)
//{
//	const char* name = typeid(t).name();
//	mVec.push_back(name);
//}
//
//struct testStruct
//{
//	int a;
//};
//
//int main()
//{
//	std::string s = "     ";
//	COstringstream os;
//	os << s;
//	os << 105;
//	os << s;
//	os << "          ";
//	os << 23;
//
//	testStruct pVoid;
//	pVoid.a = 10;
//
//	std::string str = os;
//	CIstringstream is(str);
//	std::string str1;
//	is >> str1;
//	int a = 0;
//	is >> a;
//	char buf[1024] = "";
//	is >> buf;
//	std::string str2;
//	is >> str2;
//	is >> a;
//
//	return 0;
//}

//
//struct MyStruct
//{
//	int id;
//	int level;
//};
//
//int main()
//{
//	MyStruct msg;
//
//	msg.id = 10;
//	msg.level = 99;
//
//	std::unique_ptr<char[]> uniqueBuf(new char[sizeof(MyStruct)]);
//	{
//		memcpy(uniqueBuf.get(), &msg, sizeof(SocketReadLine));
//	}
//	MyStruct* p = (MyStruct*)uniqueBuf.get();
//
//	std::cout << p->id << std::endl << "level:" << p->level << std::endl;
//
//	return 0;
//}
//
//#pragma comment(lib,"ws2_32.lib")
//
//static int StreamSocketpair(struct addrinfo* addr_info, SOCKET sock[2]) 
//{
//	SOCKET listener, client, server;
//	int opt = 1;
//
//	listener = server = client = INVALID_SOCKET;
//	listener = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol); //创建服务器socket并进行绑定监听等
//	if (INVALID_SOCKET == listener)
//		goto fail;
//
//	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
//
//	if (SOCKET_ERROR == bind(listener, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)))
//		goto fail;
//
//	if (SOCKET_ERROR == getsockname(listener, addr_info->ai_addr, (int*)&addr_info->ai_addrlen))
//		goto fail;
//
//	if (SOCKET_ERROR == listen(listener, 5))
//		goto fail;
//
//	client = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol); //创建客户端socket，并连接服务器
//
//	if (INVALID_SOCKET == client)
//		goto fail;
//
//	if (SOCKET_ERROR == connect(client, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)))
//		goto fail;
//
//	server = accept(listener, 0, 0);
//
//	if (INVALID_SOCKET == server)
//		goto fail;
//
//	closesocket(listener);
//
//	sock[0] = client;
//	sock[1] = server;
//
//	return 0;
//fail:
//	if (INVALID_SOCKET != listener)
//		closesocket(listener);
//	if (INVALID_SOCKET != client)
//		closesocket(client);
//	return -1;
//}
//
//static int DgramSocketpair(struct addrinfo* addr_info, SOCKET sock[2])
//{
//	SOCKET client, server;
//	struct addrinfo addr, * result = NULL;
//	const char* address;
//	int opt = 1;
//
//	server = client = INVALID_SOCKET;
//
//	server = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol);
//	if (INVALID_SOCKET == server)
//		goto fail;
//
//	setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
//
//	if (SOCKET_ERROR == bind(server, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)))
//		goto fail;
//
//	if (SOCKET_ERROR == getsockname(server, addr_info->ai_addr, (int*)&addr_info->ai_addrlen))
//		goto fail;
//
//	client = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol);
//	if (INVALID_SOCKET == client)
//		goto fail;
//
//	memset(&addr, 0, sizeof(addr));
//	addr.ai_family = addr_info->ai_family;
//	addr.ai_socktype = addr_info->ai_socktype;
//	addr.ai_protocol = addr_info->ai_protocol;
//
//	if (AF_INET6 == addr.ai_family)
//		address = "0:0:0:0:0:0:0:1";
//	else
//		address = "127.0.0.1";
//
//	if (getaddrinfo(address, "0", &addr, &result))
//		goto fail;
//
//	setsockopt(client, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
//	if (SOCKET_ERROR == bind(client, result->ai_addr, static_cast<int>(result->ai_addrlen)))
//		goto fail;
//
//	if (SOCKET_ERROR == getsockname(client, result->ai_addr, (int*)&result->ai_addrlen))
//		goto fail;
//
//	if (SOCKET_ERROR == connect(server, result->ai_addr, static_cast<int>(result->ai_addrlen)))
//		goto fail;
//
//	if (SOCKET_ERROR == connect(client, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)))
//		goto fail;
//
//	freeaddrinfo(result);
//	sock[0] = client;
//	sock[1] = server;
//	return 0;
//
//fail:
//	if (INVALID_SOCKET != client)
//		closesocket(client);
//	if (INVALID_SOCKET != server)
//		closesocket(server);
//	if (result)
//		freeaddrinfo(result);
//	return -1;
//}
//
//int Socketpair(int family, int type, int protocol, SOCKET recv[2]) 
//{
//	const char* address;
//	struct addrinfo addr_info, * p_addrinfo;
//	int result = -1;
//
//	memset(&addr_info, 0, sizeof(addr_info));
//	addr_info.ai_family = family;
//	addr_info.ai_socktype = type;
//	addr_info.ai_protocol = protocol;
//	if (AF_INET6 == family)
//	{
//		address = "0:0:0:0:0:0:0:1";
//	}
//	else
//	{
//		address = "127.0.0.1";
//	}
//
//	if (0 == getaddrinfo(address, "0", &addr_info, &p_addrinfo)) 
//	{
//		if (SOCK_STREAM == type)
//		{
//			result = StreamSocketpair(p_addrinfo, recv);   //use for tcp
//		}
//		else if (SOCK_DGRAM == type)
//		{
//			result = DgramSocketpair(p_addrinfo, recv);    //use for udp
//		}
//		freeaddrinfo(p_addrinfo);
//	}
//	return result;
//}
//
//int main()
//{
//	SOCKET recv_pair[2];       /* A socket pair to unblock select, when needed */
//
//	WSADATA wsaData;
//	WORD socketVersion = MAKEWORD(2, 2);
//	if (WSAStartup(socketVersion, &wsaData) != 0)
//	{
//		printf("Init socket dll err");
//	}
//	if (Socketpair(AF_INET, SOCK_STREAM, 0, recv_pair) < 0)
//	{
//		printf("Error setting Socket pair...");
//	}
//
//	send(recv_pair[0], "hello word!!!\n", 15, 0);
//	char buf[16] = "";
//	recv(recv_pair[1], buf, 16, 0);
//
//	COUT_LOG(LOG_CINFO, buf);
//	WSACleanup();
//	return 0;
//}






//#include <iostream>
//#include <event2/event.h>
//#include <functional>
//#include <list>
//#include "../kernel/Log.h"
//#include "../kernel/LogManager.h"
//#include "../kernel/ConditionVariable.h"
//
//typedef std::list<int> GList;
//
//int main()
//{
//	CGameLogManage::Instance()->SetLogFileType(SERVICE_TYPE_DB);
//	GList gList;
//	ConditionVariable condVar;
//
//	COUT_LOG(LOG_INFO, "刘壮的DB服务器开始编写了");
//	std::unique_lock<std::mutex> uniqLock(condVar.GetMutex());
//	condVar.Wait(uniqLock, [&gList] { if (!gList.empty()) { return true; } return false; });
//	return 0;
//}



//#include <iostream>
//#include <event2/event.h>
//
//int main(int argc, char** argv) 
//{
//	//windowns 初始化socket库
//	WSADATA wsa;
//	WSAStartup(MAKEWORD(2, 2), &wsa);
//
//	event_base* base = event_base_new();
//	if (base) {
//		std::cout << "first libevent" << std::endl;
//	}
//	return 0;
//}
//
//
//#include <iostream>
//#include <mysql.h>
//
//#pragma comment(lib, "libmysql")
//int main()
//{
//	//初始化
//	MYSQL* mysql = mysql_init(NULL);
//	if (mysql == NULL)
//	{
//		printf("初始化失败%s", mysql_error(mysql));
//		return EXIT_FAILURE;
//	}
//	//改变字符集为utf-8
//	system("chcp 65001");
//
//	mysql_set_character_set(mysql, "utf8");
//
//	MYSQL* contMysql = mysql_real_connect(mysql,
//		"127.0.0.1",//地址
//		"root",//账号
//		"",//密码
//		"test_lz",//数据库
//		3366,//端口
//		NULL,
//		0
//	);
//
//	if (contMysql == NULL)
//	{
//		printf("链接失败%s", mysql_error(mysql));
//		return EXIT_FAILURE;
//	}
//
//	std::cout << "Connect success\n";
//
//
//	//创建数据库
//	int ret = mysql_query(contMysql, "create database If Not Exists myNewDatabase");
//	if (ret)
//	{
//		printf("创建数据库失败%s", mysql_error(mysql));
//		return EXIT_FAILURE;
//	}
//
//	//创建表
//	ret = mysql_query(contMysql, "create table If Not Exists   myNewDatabase.T_STUDENT (sid varchar(50),sname varchar(50))");
//
//	if (ret)
//	{
//		printf("创建表失败%s", mysql_error(mysql));
//		return EXIT_FAILURE;
//	}
//
//	//增加数据
//	ret = mysql_query(contMysql, "create table If Not Exists   myNewDatabase.T_STUDENT (sid varchar(50),sname varchar(50))");
//
//	if (ret)
//	{
//		printf("创建表失败%s", mysql_error(mysql));
//		return EXIT_FAILURE;
//	}
//
//
//	//插入
//	ret = mysql_query(contMysql, "insert into   myNewDatabase.T_STUDENT values('s002','你好')");
//
//	if (ret)
//	{
//		printf("插入失败%s", mysql_error(mysql));
//		return EXIT_FAILURE;
//	}
//
//	//查询
//	ret = mysql_query(contMysql, "select * from   myNewDatabase.T_STUDENT ");
//
//	if (ret)
//	{
//		printf("Query faliure %s\n", mysql_error(mysql));
//		return EXIT_FAILURE;
//	}
//	else {
//		//保存查询的结果
//		MYSQL_RES* res = mysql_store_result(contMysql);
//
//		MYSQL_FIELD* field;
//		//查询有哪些字段这里返回 sid sname
//		while ((field = mysql_fetch_field(res)))
//		{
//			printf(" %s\t", field->name);
//		}
//
//		unsigned int  num_fields = mysql_num_fields(res);
//		printf("\r\n");
//		MYSQL_ROW row;
//		while (row = mysql_fetch_row(res))
//		{
//			for (size_t i = 0; i < num_fields; i++)
//			{
//				printf("%s ", row[i]);
//
//			}
//			printf("\r\n");
//		}
//
//
//		mysql_free_result(res);
//	}
//
//	mysql_close(contMysql);
//
//
//	return 0;
//}
//
