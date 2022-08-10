#pragma once

#define DUtil		Util::Instance()
#define LuaMgr		CLuaMgr::Instance()
#define LogMgr		CGameLogManage::Instance()
#define CfgMgr		LuaMgr->GetConfigMgr()
#define BaseCfgMgr	CfgMgr->GetCBaseCfgMgr()

#define DTCPC		DUtil.GetTCPClient()
#define DPPC		DTCPC->GetPlayerPrepClient()
#define DSC			DPPC->GetSceneClient()
#define DPCC		DSC->GetPlayerCenterClient()

// �жϴ�С����
#define Min_(x,y) ((x)>(y)?(y):(x))
#define Max_(x,y) ((x)>(y)?(x):(y))

// ��������ά��
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

// ������Ϣע��
#define RegisterNetType(obj, name, cmd) obj->AddNetTypeCallback(cmd, std::move(std::bind(&name, obj, std::placeholders::_1)))

// ע��
#define RegisterActive(obj, name, cmd) obj->AddActiveCallback(cmd, std::move(std::bind(&name, obj, std::placeholders::_1)));

// ע������
#define RegisterActiveEnter(pobj, obj, name, cmd)\
if (!pobj)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->AddActiveEnterCallback(cmd, std::move(std::bind(&name, &obj, std::placeholders::_1)));\
}

// ע���˳�
#define RegisterActiveExit(pobj, obj, name, cmd)\
if (!pobj)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->AddActiveExitCallback(cmd, std::move(std::bind(&name, &obj, std::placeholders::_1)));\
}

// ��ȫɾ��ָ��
#define SafeDelete(pData)\
{\
	try\
	{\
		delete pData;\
	}\
	catch (...)\
	{\
		CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR");\
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
			CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR");\
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
			CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR");\
		}\
		pData = nullptr;\
	}\
}

// ��־��ӡ
#define COUT_LOG(ERRTYPE, ...)\
if (ERRTYPE == LOG_ERROR)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_ERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (ERRTYPE == LOG_INFO)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_INFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (ERRTYPE == LOG_WARN)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_WARN, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (ERRTYPE == LOG_CERROR)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (ERRTYPE == LOG_CINFO)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CINFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else if (ERRTYPE == LOG_ERROR_SYS)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_ERROR_SYS, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
}\
else\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
}

// ע����Ϸ����ص�
#define RegisterAttrs(pobj, obj, name)\
if (!pobj)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->AddAttrsCallback(std::move(std::bind(&name, obj)));\
}

// ע������Э��
#define RegisterNetwk(pobj, obj, name, cmd)\
if (!pobj)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->AddNetCallback(cmd, std::move(std::bind(&name, obj, std::placeholders::_1)));\
}

// ע�����ݿ�ص�
#define RegisterMysql(pobj, obj, name, sql)\
if (!pobj)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->AddMysqlCallback(sql, std::move(std::bind(&name, obj, std::placeholders::_1)));\
}

// ע�ᴴ�����ݿ�
#define RegisterCreat(pobj, name)\
if (!pobj)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->CreateTableI(name);\
}

#define RegisterCreatS(pobj, name)\
if (!pobj)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->CreateTableS(name);\
}

// ע�ᶨʱ��
#define RegisterTimer(pobj, obj, name, cmd, uElapse, timerType)\
if (!pobj)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��"); \
}\
else\
{\
	pobj->SetTimer(cmd, uElapse, timerType);\
	pobj->AddTimerCallback(cmd, std::move(std::bind(&name, obj)));\
}

// ��ע�ᶨʱ��
#define UnRegisterTimer(pobj, cmd)\
if (!pobj)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��"); \
}\
else\
{\
	pobj->KillTimer(cmd);\
	pobj->DelTimerCallback(cmd);\
}

// �������
#define AddAttributes(pobj, attrs)\
if (!pobj)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "�������ʧ��");\
}\
else\
{\
	pobj->AdditionAttributes(attrs);\
}

// ע����Ϸ�˳�
#define RegisterLgout(pobj, obj, name)\
if (!pobj)\
{\
	CLog::Write(LogMgr->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->AddExitCallback(std::move(std::bind(&name, obj, std::placeholders::_1)));\
}

