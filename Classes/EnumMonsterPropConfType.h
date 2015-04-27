/*
    文件名：    EnumMonsterPropConfType.h
    描　述：    怪物配置文件属性类型
    创建人：    笨木头_钟迪龙 (博客：http://www.benmutou.com)
*/
#ifndef _EnumMonsterPropConfType_H_
#define _EnumMonsterPropConfType_H_

enum EnumMonsterPropConfType {
	enMonsterPropConf_ID,		// 怪物ID
	enMonsterPropConf_Name,		// 怪物名字
    enMonsterPropConf_Level,	// 怪物等级
	enMonsterPropConf_Type,		// 怪物类型
	enMonsterPropConf_ModelID,	// 怪物模型ID
	enMonsterPropConf_Atk,
	enMonsterPropCOnf_AtkSpeed,
	enMonsterPropConf_AtkRange,
	enMonsterPropConf_Defense,	// 防御力
	enMonsterPropConf_Hp,	    // 血量
	enMonsterPropConf_Speed,	// 移动速度
	enMonsterPropConf_MetalAtk,
	enMonsterPropConf_MetalDefense,
	enMonsterPropConf_WoodAtk,
	enMonsterPropConf_WoodDefense, 
	enMonsterPropConf_WaterAtk,
	enMonsterPropConf_WaterDefense, 
	enMonsterPropConf_FireAtk,
	enMonsterPropConf_FireDefense, 
	enMonsterPropConf_EarthAtk,
	enMonsterPropConf_EarthDefense,
};

#endif