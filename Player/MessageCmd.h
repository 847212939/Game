#pragma once

// 玩家消息 对齐用空格啊
enum class MsgCmd
{
	
	MsgCmd_Begin                       = 0, // 消息开始
	MsgCmd_HeartBeat                   = 1,	// 心跳
	MsgCmd_Testlink                    = 2, // 连接测试
	MsgCmd_RegisterAccount             = 3,	// 账号注册
	MsgCmd_End                              // 消息结束                      
};