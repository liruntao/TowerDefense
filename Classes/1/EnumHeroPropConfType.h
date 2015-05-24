/*
大概类似于#define
如果没有赋值，则第一个为0，其他的依次累加，如果赋值，以此为基础依次累加。
可以直接使用。
*/
#ifndef _EnumHeroPropConfType_H_
#define _EnumHeroPropConfType_H_

enum EnumHeroPropConfType {
	enHeroPropConf_ID,		// 英雄ID
	enHeroPropConf_Name,		// 英雄名字
	enHeroPropConf_Type,		// 英雄类型
	enHeroPropConf_ModelID,		// 英雄模型ID
	enHeroPropConf_BaseAtk,			// 基础攻击力
	enHeroPropConf_AtkSpeed,		// 攻击间隔（单位：秒）
	enHeroPropConf_AtkRange,		// 攻击范围（半径）
	enHeroPropConf_UpgradeAtkBase,	// 升级攻击加成系数
	enHeroPropConf_UpgradeCostBase,	// 升级消耗基础值
	
};

#endif