/*
    �ļ�����    Hero.h
    �衡����    Ӣ�ۣ�������
    �����ˣ�    ��ľͷ_�ӵ��� (���ͣ�http://www.benmutou.com)
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

    /* ����Ӣ��ID���������ļ��ж�ȡӢ������ */
	static Hero* createFromCsvFileByID(int iHeroID,int iBulletID);
	bool initFromCsvFileByID(int iHeroID, int iBulletID);

	static Hero* createForSelect(int iHeroID, int iBulletID);
	bool initForSelect(int iHeroID, int iBulletID);
	void Null();
	
	CC_SYNTHESIZE(int, m_iCurAtk, iCurAtk);				    //��ǰ������

 

    /* ����Ӣ�� */
    void upgrade();

    void checkAtkMonster(float ft, Vector<Monster*> monsterList);
	int m_bulletType;
	BulletMulti* m_bullet1;

private:
    /* �����Ƿ���ȴ */
    bool m_isAtkCoolDown;


    /* ��ǰ�����Ĺ��� */
    Monster* m_atkMonster;
    
    /* �ӵ������� */
    BulletManager* m_bulletMgr;

    /* ��Ⲣѡ����빥����Χ�Ĺ�����߹����ѽ��뷶Χ�Ĺ��� */
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