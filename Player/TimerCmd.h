#pragma once

// ��ʱ����Ϣ
enum class TimerCmd
{
	TimerCmd_Begin						= 0,		// ��Ϣ��ʼ
	TimerCmd_Test						= 1,		// ���Զ�ʱ��
	TimerCmd_Active						= 2,		// �
	TimerCmd_Skill						= 3,		// ����CD

	TimerCmd_End								// ��Ϣ����                
};