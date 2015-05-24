#ifndef __GLOBAL_DEFINE_H__
#define __GLOBAL_DEFINE_H__
/*
没用
*/

#include "cocos2d.h"

/* 创建bool变量，包括get和set方法 */
#define CC_SYNTHESIZE_BOOL(varName, funName)\
protected: bool varName; \
public: bool is##funName(void) const { return varName; }\
public: void set##funName(bool var){ varName = var; }


/* 消息派发 */
#define NOTIFY cocos2d::NotificationCenter::getInstance()
static const int DRAG_BODYS_TAG = 0x80;

//碰撞检测时，各种物体的tag
static const int BULLET_ONETAG = 100;
static const int BULLET_SEVERALTAG = 101;
static const int MONSTERTAG = 102;
static const int MONSTERBODYTAG = 103;

//碰撞检测bit值
//一个body的CategoryBitmask和另一个body的ContactTestBitmask的逻辑与的结果不等于0时，接触事件将被发出，否则不发送。
//一个body的CategoryBitmask和另一个body的CollisionBitmask的逻辑与结果不等于0时，会碰撞，否则不碰撞
//setCategoryBitmask(0x08);    // 1000
//setContactTestBitmask(0x08); // 0010
//setCollisionBitmask(0x01);// 0001


//设计hero与monster和monsterBullet会发生碰撞及调用碰撞检测，hero和hero会发生碰撞，hero和城墙会发生碰撞,hero与herobullet既不发生碰撞，也不发生回调函数
//hero
static const int HERO_CATEGORY_BITMASK = 0x08;//1000
static const int HERO_CONTACTTEST_BITMASK = 0x01;//0001
static const int HERO_COLLISION_BITMASK = 0x01;//0001

//monster
static const int MONSTER_CATEGORY_BITMASK = 0x08;//1000
static const int MONSTER_CONTACTTEST_BITMASK = 0x08;//1000
static const int MONSTER_COLLISION_BITMASK = 0x08;//0001

//heroBullet
static const int HEROBULLET_CATEGORY_BITMASK = 0x08;//1000
static const int HEROBULLET_CONTACTTEST_BITMASK = 0x08;//0010
static const int HEROBULLET_COLLISION_BITMASK = 0x01;//0001

//monsterBullet
static const int MONSTERBULLET_CATEGORY_BITMASK = 0x08;//1000
static const int MONSTERBULLET_CONTACTTEST_BITMASK = 0x08;//0010
static const int MONSTERBULLET_COLLISION_BITMASK = 0x01;//0001

//monsterBody
static const int MONSTERBODY_CATEGORY_BITMASK = 0x08;//1000
static const int MONSTERBODY_CONTACTTEST_BITMASK = 0x08;//1000
static const int MONSTERBODY_COLLISION_BITMASK = 0x01;//0001
#endif