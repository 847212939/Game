#pragma once

// �����Ϣ �����ÿո�
enum class MsgCmd
{
	MsgCmd_Begin                       = 0, // ��Ϣ��ʼ
	MsgCmd_HeartBeat                   = 1,	// ����
	MsgCmd_Testlink                    = 2, // ���Ӳ���
	MsgCmd_Login				       = 3,	// ��ҵ�¼
	MsgCmd_Move                        = 4,	// �ƶ�

	MsgCmd_End                              // ��Ϣ����                      
};