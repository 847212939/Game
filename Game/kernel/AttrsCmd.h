#pragma once

enum class AttrsCmd
{
	attrs_begin = 0,

	attrs_hp	= 1,		// 血量
	attrs_bp	= 2,		// 蓝
	attrs_ys	= 3,		// 移速
	attrs_jsp	= 4,		// 移速百分比
	attrs_pa	= 5,		// 物理攻击
	attrs_dpa	= 6,		// 物理防御
	attrs_ma	= 7,		// 魔法攻击
	attrs_dma	= 8,		// 魔法防御
	attrs_cs	= 9,		// 暴击概率
	attrs_pp	= 10,		// 物理穿透
	attrs_mp	= 11,		// 魔法穿透
	attrs_prd	= 12,		// 真实伤害百分比
	attrs_gs	= 13,		// 攻速
	attrs_pxx	= 14,		// 物理吸血
	attrs_dpxx	= 15,		// 物理吸血抵抗
	attrs_mxx	= 16,		// 魔法吸血
	attrs_dmxx	= 17,		// 魔法吸血抵抗
	attrs_fs	= 18,		// 反伤(值针对物理攻击)
	attrs_cm	= 19,		// 沉默
	attrs_jf	= 20,		// 击飞
	attrs_jg	= 21,		// 禁锢
	attrs_scd	= 22,		// 减技能CD
	attrs_secd	= 23,		// 减技能效果CD

	attrs_end
};