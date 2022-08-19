#pragma once

// 玩家消息
enum class MsgCmd
{
	MsgCmd_Begin						= 0,	// 消息开始

	MsgCmd_HeartBeat					= 1,	// 心跳
	MsgCmd_Testlink						= 2,	// 连接测试
	MsgCmd_PlayerPreproces				= 3,	// 玩家预处理
	MsgCmd_Scene						= 4,	// 场景消息
	MsgCmd_PlayerCenter					= 5,	// 玩家中心消息
	MsgCmd_Timer						= 6,	// 定时器
	MsgCmd_Login						= 7,	// 登录
	MsgCmd_RefreshProperties			= 8,	// 属性刷新
	MsgCmd_Move							= 9,	// 移动系统
	MsgCmd_Hurt							= 10,	// 伤害系统
	MsgCmd_Skill						= 11,	// 技能系统

	MsgCmd_End									// 消息结束                      
};
