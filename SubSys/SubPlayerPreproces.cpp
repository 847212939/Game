#include "../Game/stdafx.h"

SubPlayerPreproces::SubPlayerPreproces(TCPClient* pTCPClient) :
	PlayerPreproces(pTCPClient)
{
	CallBackFunInit();
}

SubPlayerPreproces::~SubPlayerPreproces()
{
}

// ��ʼ����Ϣ�ص�����
void SubPlayerPreproces::CallBackFunInit()
{
	AddCallBackFun(MsgCmd::MsgCmd_Login, std::move(std::bind(&SubPlayerPreproces::LoginInAccount, this, std::placeholders::_1)));
}

// ��¼
void SubPlayerPreproces::LoginInAccount(PlayerInfo* pPlayerInfo)
{
	if (!pPlayerInfo)
	{
		COUT_LOG(LOG_CERROR, "!pPlayerInfo");
		return;
	}
	if (!pPlayerInfo->m_pMsg || !pPlayerInfo->m_pTcpSockInfo)
	{
		COUT_LOG(LOG_CERROR, "!pPlayerInfo->pMsg || !pPlayerInfo->pTcpSockInfo");
		return;
	}
	if (!pPlayerInfo->m_pTcpSockInfo->isConnect)
	{
		COUT_LOG(LOG_CERROR, "Network link closed");
		return;
	}

	unsigned int uAssistantID = pPlayerInfo->m_pMsg->netMessageHead.uAssistantID;
	std::string str = (char*)pPlayerInfo->m_pData;
	CIstringstream is(str);
	std::string id, pw;
	is >> id >> pw;

	if (SubPPMsgCmd::CS_Login == (SubPPMsgCmd)uAssistantID)
	{
		if (LoginIn(id, pw, pPlayerInfo))
		{
			CreatePlayer(pPlayerInfo->m_pMsg->uIndex, pPlayerInfo->m_pTcpSockInfo, pPlayerInfo->m_userId);
		}
	}
}

// ��¼
bool SubPlayerPreproces::LoginIn(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo)
{
	if (id.empty() || passwaed.empty())
	{
		// �˺Ų�����
		COUT_LOG(LOG_CINFO, "id or passworld is empty");
		return false;
	}

	std::string userId;

	// ���ݿ��ѯ
	std::string pw = LoadUserAccount(id);
	if (pw.empty())
	{
		userId = Util::CreateUuid();

		SaveReplaceSQL("useraccount", id, passwaed);
		SaveReplaceSQL("userid", id, userId);

		pPlayerInfo->m_userId = userId;
		return true;
	}
	else
	{
		if (pw != passwaed)
		{
			// ���벻��ȷ
			COUT_LOG(LOG_CINFO, "passworld is error");
			return false;
		}
		else
		{
			// ������ȷ
			userId = LoadUserId(id);
			if (userId.empty())
			{
				userId = Util::CreateUuid();

				SaveReplaceSQL("userid", id, userId);
				pPlayerInfo->m_userId = userId;
				return true;
			}
		}
	}

	pPlayerInfo->m_userId = userId;
	return true;
}

// �������userid
std::string SubPlayerPreproces::LoadUserId(std::string& id)
{
	CMysqlHelper::MysqlData data;
	LoadOneSql(id, "userid", data);
	if (data.size() > 0)
	{
		SqlKeyDataMap& dataMap = data[0];
		SqlKeyDataMap::iterator it = dataMap.find("data");
		if (it != dataMap.end())
		{
			return it->second;
		}
	}
	return "";
}

// ��������˺���Ϣ
std::string SubPlayerPreproces::LoadUserAccount(std::string& id)
{
	CMysqlHelper::MysqlData data;
	LoadOneSql(id, "useraccount", data);
	if (data.size() > 0)
	{
		SqlKeyDataMap& dataMap = data[0];
		SqlKeyDataMap::iterator it = dataMap.find("data");
		if (it != dataMap.end())
		{
			return it->second;
		}
	}
	return "";
}
