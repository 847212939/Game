#pragma once

#define DUtil		Util::Instance()
#define LuaMgr		CLuaMgr::Instance()
#define CfgMgr		LuaMgr->GetConfigMgr()
#define BaseCfgMgr	CfgMgr->GetCBaseCfgMgr()

// Get Set mem
#define GetSetMem(type,Name)\
private:\
	type m_##Name;\
public:\
	type Get##Name() const\
	{\
		return this->m_##Name;\
	}\
public:\
	void Set##Name(type _arg)\
	{\
		this->m_##Name=_arg;\
	}\
private:

// 类型返回以引用方式
#define GetClass(type,Name)\
private:\
	type m_##Name;\
public:\
	type& Get##Name()\
	{\
		return this->m_##Name;\
	}\
private:

// 判断大小函数
#define Min_(x,y) ((x)>(y)?(y):(x))
#define Max_(x,y) ((x)>(y)?(x):(y))

// 注册创建数据库
#define RegisterTable(name) this->CreateTableI(name);
#define RegisterCreate(name) this->CreateTableS(name);

// 计算数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

// 网络消息注册
#define RegisterNetType(name, cmd) this->AddNetTypeCallback(cmd, std::move(std::bind(&name, this, std::placeholders::_1)))

// 注册活动
#define RegisterActive(name, cmd) this->AddActiveCallback(cmd, std::move(std::bind(&name, this, std::placeholders::_1)));

// 注册活动进入
#define RegisterActiveEnter(name, cmd, obj) this->AddActiveEnterCallback(cmd, std::move(std::bind(&name, &obj, std::placeholders::_1)));

// 注册活动退出
#define RegisterActiveExit(name, cmd, obj) this->AddActiveExitCallback(cmd, std::move(std::bind(&name, &obj, std::placeholders::_1)));

// 安全删除指针
#define SafeDelete(pData)\
{\
	try\
	{\
		delete pData;\
	}\
	catch (...)\
	{\
		std::cout << "DELETE ERR" << std::endl;\
	}\
	pData = nullptr;\
} 

// 安全删除指针
#define  SafeDeleteArray(pData)\
{\
	if (pData)\
	{\
		try\
		{\
			delete[] pData;\
		}\
		catch (...)\
		{\
			std::cout << "DELETE ERR" << std::endl;\
		}\
	}\
}

// 安全删除指针
#define SAFE_DELETE(pData)\
{\
	if(pData)\
	{\
		try\
		{\
			delete pData;\
		}\
		catch(...)\
		{\
			std::cout << "DELETE ERR" << std::endl;\
		}\
		pData = nullptr;\
	}\
}

// 注册网络协议
#define RegisterNetwk(pobj, name, cmd)\
if (!pobj)\
{\
	std::cout << "注册消息失败 请检查写法" << std::endl;\
}\
else\
{\
	pobj->AddNetCallback(cmd, std::move(std::bind(&name, this, std::placeholders::_1)));\
}

// 注册定时器
#define RegisterTimer(pobj, name, cmd, uElapse, timerType)\
if (!pobj)\
{\
	std::cout << "注册消息失败 请检查写法" << std::endl;\
}\
else\
{\
	pobj->SetTimer(cmd, uElapse, timerType);\
	pobj->AddTimerCallback(cmd, std::move(std::bind(&name, this)));\
}

// 反注册定时器
#define UnRegisterTimer(pobj, cmd)\
if (!pobj)\
{\
	std::cout << "注册消息失败 请检查写法" << std::endl;\
}\
else\
{\
	pobj->KillTimer(cmd);\
	pobj->DelTimerCallback(cmd);\
}
