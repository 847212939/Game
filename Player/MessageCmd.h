#pragma once

// �����Ϣ
enum class MsgCmd
{
	MsgCmd_Begin                       = 0, // ��Ϣ��ʼ

	MsgCmd_HeartBeat                   = 1,	// ����
	MsgCmd_Testlink                    = 2, // ���Ӳ���
	MsgCmd_Login				       = 3,	// ��ҵ�¼
	MsgCmd_Scene                       = 4, // ������Ϣ
	MsgCmd_PlayerCenter                = 5, // ���������Ϣ
	MsgCmd_Move                        = 6,	// �ƶ�

	MsgCmd_End                              // ��Ϣ����                      
};