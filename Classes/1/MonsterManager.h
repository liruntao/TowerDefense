/*
    文件名：    MonsterManager.h
    描　述：    怪物管理器
    创建人：    笨木头_钟迪龙 (博客：http://www.benmutou.com)

    创建日期：   2013.05.04
*/
#ifndef _MonsterManager_H_
#define _MonsterManager_H_

class TollgateMapLayer;

#include "cocos2d.h"
USING_NS_CC;

class MonsterPos;
class Monster;
class PosBase;
class MonsterManager : public Node {
public:
    MonsterManager();
    ~MonsterManager();
    static MonsterManager* createWithLevel(int iCurLevel,TollgateMapLayer* layer);
	bool initWithLevel(int iCurLevel, TollgateMapLayer* layer);

	void createMonsters(int iCurLevel, TollgateMapLayer* layer);    /* 读取配置文件创建怪物 */
    int getNotShowMonsterCount();          /* 获取还没有出场的怪物数量 */
    MonsterPos* getMonsterStartPos();      /* 获取怪物出场坐标 */
    MonsterPos* getMonsterEndPos();        /* 获取怪物终点坐标 */
    Vector<Monster*> getMonsterList();     /* 获取怪物列表 */

private:
    Vector<Monster*> m_monsterList;        /* 怪物列表 */
    Vector<Monster*> m_notShowMonsterList; /* 未出场的怪物列表 */
    Vector<PosBase*> m_monsterPosList;  /* 存放所有怪物的坐标对象 */
    float m_fShowTimeCount;                /* 用于计算怪物出场时间 */
    void showMonster(float dt);            /* 检查是否有新怪物出场 */
    void logic(float dt);                  /* 检测怪物是否需要删除 */
	TollgateMapLayer* _layer;
};

#endif