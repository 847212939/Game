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
	MsgCmd_RefreshProperties           = 6, // ����ˢ��
	MsgCmd_Move                        = 7,	// �ƶ�
	MsgCmd_Timer                       = 8,	// ��ʱ��

	MsgCmd_End                              // ��Ϣ����                      
};
