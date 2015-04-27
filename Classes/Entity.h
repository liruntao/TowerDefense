/*
实体基类，继承自node，这里有bindsprite，也可以bind其他的东西。
操作起来，比把英雄、monster什么的直接继承自sprite要好，要更灵活，可以绑定更多的东西。

这个基类中是否需要把析构函数定义为virtual？？
*/

#ifndef _Entity_H_
#define _Entity_H_

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"
using namespace cocostudio;
USING_NS_CC;

class Entity : public Node {
public:
    Entity();
    ~Entity();

    void bindSprite(Sprite* sprite);
    Sprite* getSprite();

	void bindArmature(Armature *armature);
	Armature* getArmature();
	Armature* m_armature;

    void hurtMe(int iHurtValue);        /* 被攻击 */
    bool isDead();                      /* 是否死亡 */


protected:
    virtual void onDead();		        /* 实体死亡时调用 */
    virtual void onBindSprite();        /* 绑定精灵对象时调用 */
    virtual void onHurt(int iHurtValue);/* 受伤害时调用 */
private:
    Sprite* m_sprite;

    CC_SYNTHESIZE(int, m_ID, ID);	            //实体ID
    CC_SYNTHESIZE(int, m_iModelID, iModelID);	//模型ID（资源ID）
    CC_SYNTHESIZE(std::string, m_sName, sName);	//名字
    CC_SYNTHESIZE(int, m_iHP, iHP);			    //HP
    CC_SYNTHESIZE(int, m_iDefense, iDefense);	//防御
    CC_SYNTHESIZE(int, m_iSpeed, iSpeed);		//移动速度
    CC_SYNTHESIZE(int, m_iLevel, iLevel);		//等级
	CC_SYNTHESIZE(int, m_iBaseAtk, iBaseAtk);				//基础攻击力，没有用
	CC_SYNTHESIZE(int, m_iAtkSpeed, iAtkSpeed);				//攻击间隔（单位：毫秒）
	CC_SYNTHESIZE(int, m_iAtkRange, iAtkRange);				//攻击范围（半径）
	CC_SYNTHESIZE(int, m_iUpgradeCostBase, iUpgradeCostBase);	//升级消耗基础值
	CC_SYNTHESIZE(float, m_fUpgradeAtkBase, fUpgradeAtkBase);	//升级攻击加成系数
	CC_SYNTHESIZE(int, cgHp, changeHp);   
	CC_SYNTHESIZE(int, cgDefense, changeDefense);
	CC_SYNTHESIZE(int, cgSpeed, changeSpeed);
	CC_SYNTHESIZE(int, bodycgSpeed, changebodySpeed);
//以下为属性设计
	CC_SYNTHESIZE(int, m_MetalAtk, MetalAtk);
	CC_SYNTHESIZE(int, m_MetalDefense, MetalDefense);
	CC_SYNTHESIZE(int, m_WoodAtk, WoodAtk);
	CC_SYNTHESIZE(int, m_WoodDefense, WoodDefense);
	CC_SYNTHESIZE(int, m_WaterAtk, WaterAtk);
	CC_SYNTHESIZE(int, m_WaterDefense, WaterDefense);
	CC_SYNTHESIZE(int, m_FireAtk, FireAtk);
	CC_SYNTHESIZE(int, m_FireDefense, FireDefense);
	CC_SYNTHESIZE(int, m_EarthAtk, EarthAtk);
	CC_SYNTHESIZE(int, m_EarthDefense, EarthDefense);


    bool m_isDead;      //标记是否死亡
};

#endif