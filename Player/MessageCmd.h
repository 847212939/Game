#pragma once

// �����Ϣ �����ÿո�
enum class MsgCmd
{
	
	MsgCmd_Begin                       = 0, // ��Ϣ��ʼ
	MsgCmd_HeartBeat                   = 1,	// ����
	MsgCmd_Testlink                    = 2, // ���Ӳ���
	MsgCmd_RegisterAccount             = 3,	// �˺�ע��
	MsgCmd_End                              // ��Ϣ����                      
};