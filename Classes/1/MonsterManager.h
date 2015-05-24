/*
    �ļ�����    MonsterManager.h
    �衡����    ���������
    �����ˣ�    ��ľͷ_�ӵ��� (���ͣ�http://www.benmutou.com)

    �������ڣ�   2013.05.04
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

	void createMonsters(int iCurLevel, TollgateMapLayer* layer);    /* ��ȡ�����ļ��������� */
    int getNotShowMonsterCount();          /* ��ȡ��û�г����Ĺ������� */
    MonsterPos* getMonsterStartPos();      /* ��ȡ����������� */
    MonsterPos* getMonsterEndPos();        /* ��ȡ�����յ����� */
    Vector<Monster*> getMonsterList();     /* ��ȡ�����б� */

private:
    Vector<Monster*> m_monsterList;        /* �����б� */
    Vector<Monster*> m_notShowMonsterList; /* δ�����Ĺ����б� */
    Vector<PosBase*> m_monsterPosList;  /* ������й����������� */
    float m_fShowTimeCount;                /* ���ڼ���������ʱ�� */
    void showMonster(float dt);            /* ����Ƿ����¹������ */
    void logic(float dt);                  /* �������Ƿ���Ҫɾ�� */
	TollgateMapLayer* _layer;
};

#endif