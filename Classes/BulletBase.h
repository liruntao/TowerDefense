/*
    文件名：    BulletBase.h
    描　述：    子弹基类
    创建人：    笨木头_钟迪龙 (博客：http://www.benmutou.com)

	设定子弹的攻击力和攻击速度，判断是否锁定目标，如果锁定则将子弹的目标m_aim=锁定的目标，
	还可以获取该子弹的目标getaim（），声明一个变量判断子弹是否到达目的地，声明变量子弹是否正在使用中（在同一时间只可被使用一次）
	可以获得使用状态和设定是否使用。

*/
#ifndef _BulletBase_H_
#define _BulletBase_H_

#include "Entity.h"

/* 子弹速度 */
#define SPEED_DEFAULT 10
#define SPEED_NORMAL 5

class BulletBase : public Entity {
public:
    BulletBase();
    ~BulletBase();

    /* 锁定攻击目标 */
    void lockAim(Entity* entity);

    /* 获取攻击目标 */
    Entity* getAim();

	/* 是否正在使用 */
    void setUsed(bool isUsed);
    bool isUsed();

	/* 是否到达目标 */
    bool isArrive();
	/* 锁定攻击目标时调用，留给子类做处理 */
	virtual void onLockAim(Entity* aim) = 0;
protected:
  
	Entity* m_aim;	    /* 攻击目标 */

    bool m_isArrive;    /* 是否达到了攻击目标（是否攻击了目标） */
private:
    bool m_isUsed;      /* 标记是否已经在使用中 */

    CC_SYNTHESIZE(int, m_iAtkValue, iAtkValue); /* 攻击力 */
    CC_SYNTHESIZE(int, m_iSpeed, iSpeed);       /* 速度 */
	CC_SYNTHESIZE(float, rate, Rate);//塔的射程
	CC_SYNTHESIZE(int, type, Type);//子弹种类

};

#endif