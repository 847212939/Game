#pragma once

// 玩家消息
enum class MsgCmd
{
	MsgCmd_Begin                       = 0, // 消息开始

	MsgCmd_HeartBeat                   = 1,	// 心跳
	MsgCmd_Testlink                    = 2, // 连接测试
	MsgCmd_Login				       = 3,	// 玩家登录
	MsgCmd_Scene                       = 4, // 场景消息
	MsgCmd_PlayerCenter                = 5, // 玩家中心消息
	MsgCmd_RefreshProperties           = 6, // 属性刷新
	MsgCmd_Move                        = 7,	// 移动
	MsgCmd_Timer                       = 8,	// 定时器

	MsgCmd_End                              // 消息结束                      
};
