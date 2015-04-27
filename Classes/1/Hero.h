/*
    文件名：    Hero.h
    描　述：    英雄（炮塔）
    创建人：    笨木头_钟迪龙 (博客：http://www.benmutou.com)
*/
#ifndef _Hero_H_
#define _Hero_H_

#include "Entity.h"

class Monster;
class BulletManager;
class BulletMulti;
class Hero : public Entity {
public:
    Hero();
    ~Hero();

    static Hero* create(Sprite* sprite);
    bool init(Sprite* sprite);

    /* 给定英雄ID，从配置文件中读取英雄数据 */
	static Hero* createFromCsvFileByID(int iHeroID,int iBulletID);
	bool initFromCsvFileByID(int iHeroID, int iBulletID);

	static Hero* createForSelect(int iHeroID, int iBulletID);
	bool initForSelect(int iHeroID, int iBulletID);
	void Null();
	
	CC_SYNTHESIZE(int, m_iCurAtk, iCurAtk);				    //当前攻击力

 

    /* 升级英雄 */
    void upgrade();

    void checkAtkMonster(float ft, Vector<Monster*> monsterList);
	int m_bulletType;
	BulletMulti* m_bullet1;

private:
    /* 攻击是否冷却 */
    bool m_isAtkCoolDown;


    /* 当前锁定的怪物 */
    Monster* m_atkMonster;
    
    /* 子弹管理类 */
    BulletManager* m_bulletMgr;

    /* 检测并选择进入攻击范围的怪物，或者攻击已进入范围的怪物 */
    void checkAtkMonster(float ft);

    void chooseAim(Vector<Monster*> monsterList);
    void chooseAtkMonster(Monster* monster);
    bool isInAtkRange(Point pos);

    void atk(float ft);
    void atkCollDownEnd(float dt);

    void checkAimIsOutOfRange(Vector<Monster*> monsterList);
    void missAtkMonster();


};

#endif