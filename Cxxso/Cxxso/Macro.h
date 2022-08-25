#pragma once
#ifdef _WIN32
#else
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

// ���ͷ��������÷�ʽ
#define GetClass(type,Name)\
private:\
	type m_##Name;\
public:\
	type& Get##Name()\
	{\
		return this->m_##Name;\
	}\
private:

// �жϴ�С����
#define Min_(x,y) ((x)>(y)?(y):(x))
#define Max_(x,y) ((x)>(y)?(x):(y))

// ע�ᴴ�����ݿ�
#define RegisterTable(name) this->CreateTableI(name);
#define RegisterCreate(name) this->CreateTableS(name);

// ��������ά��
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

// ������Ϣע��
#define RegisterNetType(name, cmd) this->AddNetTypeCallback(cmd, std::move(std::bind(&name, this, std::placeholders::_1)))

// ע��
#define RegisterActive(name, cmd) this->AddActiveCallback(cmd, std::move(std::bind(&name, this, std::placeholders::_1)));

// ע������
#define RegisterActiveEnter(name, cmd, obj) this->AddActiveEnterCallback(cmd, std::move(std::bind(&name, &obj, std::placeholders::_1)));

// ע���˳�
#define RegisterActiveExit(name, cmd, obj) this->AddActiveExitCallback(cmd, std::move(std::bind(&name, &obj, std::placeholders::_1)));

// ��ȫɾ��ָ��
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

// ��ȫɾ��ָ��
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

// ��ȫɾ��ָ��
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

// ע������Э��
#define RegisterNetwk(pobj, name, cmd)\
if (!pobj)\
{\
	std::cout << "ע����Ϣʧ�� ����д��" << std::endl;\
}\
else\
{\
	pobj->AddNetCallback(cmd, std::move(std::bind(&name, this, std::placeholders::_1)));\
}

// ע�ᶨʱ��
#define RegisterTimer(pobj, name, cmd, uElapse, timerType)\
if (!pobj)\
{\
	std::cout << "ע����Ϣʧ�� ����д��" << std::endl;\
}\
else\
{\
	pobj->SetTimer(cmd, uElapse, timerType);\
	pobj->AddTimerCallback(cmd, std::move(std::bind(&name, this)));\
}

// ��ע�ᶨʱ��
#define UnRegisterTimer(pobj, cmd)\
if (!pobj)\
{\
	std::cout << "ע����Ϣʧ�� ����д��" << std::endl;\
}\
else\
{\
	pobj->KillTimer(cmd);\
	pobj->DelTimerCallback(cmd);\
}
#endif