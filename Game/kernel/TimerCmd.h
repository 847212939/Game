#pragma once

// 定时器消息
enum class TimerCmd
{
	TimerCmd_Begin						= 0,		// 消息开始
	TimerCmd_Test						= 1,		// 测试定时器
	TimerCmd_Active						= 2,		// 活动
	TimerCmd_Skill						= 3,		// 技能CD

	TimerCmd_End									// 消息结束                
};