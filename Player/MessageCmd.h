#pragma once

// �����Ϣ
enum class MsgCmd
{
	MsgCmd_Begin						= 0,	// ��Ϣ��ʼ

	MsgCmd_HeartBeat					= 1,	// ����
	MsgCmd_Testlink						= 2,	// ���Ӳ���
	MsgCmd_PlayerPreproces				= 3,	// ���Ԥ����
	MsgCmd_Scene						= 4,	// ������Ϣ
	MsgCmd_PlayerCenter					= 5,	// ���������Ϣ
	MsgCmd_Timer						= 6,	// ��ʱ��
	MsgCmd_Login						= 7,	// ��¼
	MsgCmd_RefreshProperties			= 8,	// ����ˢ��
	MsgCmd_Move							= 9,	// �ƶ�ϵͳ
	MsgCmd_Hurt							= 10,	// �˺�ϵͳ
	MsgCmd_Skill						= 11,	// ����ϵͳ

	MsgCmd_End									// ��Ϣ����                      
};
