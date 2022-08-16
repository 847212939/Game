#include "../Game/stdafx.h"

HurtSys::HurtSys(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, this, HurtSys::Network, MsgCmd::MsgCmd_Hurt);
}

HurtSys::~HurtSys()
{

}

void HurtSys::Network(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "Player Info is null");
		return;
	}
	HurtSysMsgCmd uAssistantID = (HurtSysMsgCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Cis is((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case HurtSysMsgCmd::cs_attack:
	{
		CalHurt(is, playerInfo);
		break;
	}
	default:
		break;
	}
}

bool HurtSys::CalHurt(Cis& is, PlayerInfo* playerInfo)
{
	int sceneid = 0;
	uint64_t hitedid = 0, behitedid = 0;

	is >> sceneid >> hitedid >> behitedid;

	Animal* hited = DSC->GetSceneAnimal(sceneid, hitedid);
	Animal* behited = DSC->GetSceneAnimal(sceneid, behitedid);

	return CalHurt(hited, behited);
}

// 计算伤害 @hited 打人 @behited 被打
bool HurtSys::CalHurt(Animal* hited, Animal* behited)
{
	if (!hited || !behited)
	{
		COUT_LOG(LOG_CERROR, "hited = null || behited = null");
		return false;
	}

	hited->GetAttrValue(AttrsCmd::attrs_hp);

	return true;
}
