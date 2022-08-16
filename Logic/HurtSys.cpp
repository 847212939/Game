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
	int skillid = 0;

	is >> sceneid >> hitedid >> behitedid >> skillid;

	Animal* hited = DSC->GetSceneAnimal(sceneid, hitedid);
	Animal* behited = DSC->GetSceneAnimal(sceneid, behitedid);

	return CalHurt(hited, behited);
}

// 计算伤害 @hited 打人 @behited 被打
// 伤害 = 攻击*(100÷((100+((防御-防御穿透值)*(1-防御穿透百分比)。
bool HurtSys::CalHurt(Animal* hited, Animal* behited)
{
	if (!hited || !behited)
	{
		COUT_LOG(LOG_CERROR, "hited = null || behited = null");
		return false;
	}

	return true;
}
