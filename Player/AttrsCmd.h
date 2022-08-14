#pragma once

enum class AttrsCmd
{
	attrs_begin = 0,

	attrs_hp = 1,	// 血量
	attrs_bp = 2,	// 蓝
	attrs_ms = 3,	// 移速
	attrs_pa = 4,	// 物理攻击
	attrs_ma = 5,	// 魔法攻击
	attrs_pr = 6,	// 物理抗性
	attrs_mr = 7,	// 魔法抗性
	attrs_cs = 8,	// 暴击概率
	attrs_pp = 9,	// 物理穿透
	attrs_mp = 10,	// 魔法穿透

	attrs_end
};