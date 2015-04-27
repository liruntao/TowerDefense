/*
    �ļ�����    Monster.h
    �衡����    �������
    �����ˣ�    ��ľͷ_�ӵ��� (���ͣ�http://www.benmutou.com)
*/
#ifndef _Monster_H_
#define _Monster_H_

#include "editor-support/cocostudio/CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC_EXT;

#include "Entity.h"
#include "GlobalDefine.h"
#include "ControllerAMove.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#define MONSTER_SPEED_INIT 1


class PosBase;
class ControllerAMove;
class TollgateMapLayer;
class Monster : public Entity {
public:
    Monster();
    ~Monster();
    CREATE_FUNC(Monster);
    virtual bool init();
	
	

    /* ��������ID���������ļ��ж�ȡ�������� */
	static Monster* createFromCsvFileByID(int iMonsterID, TollgateMapLayer* layer);
    bool initFromCsvFileByID(int iMonsterID,TollgateMapLayer* layer);

	//��sprite
	static Monster* create(Sprite* sprite);
	bool init(Sprite* sprite);
	ControllerAMove* getMoveController();

protected:
    virtual void onDead() override;
    virtual void onBindSprite() override;
    virtual void onHurt(int iHurtValue) override;
public:
    /* ���ո�����������ƶ� */
    void moveByPosList(Vector<PosBase*> posList);
private:
	//monster������VauleMap
	ValueMap theFiveLineProperties;
	void initValueMapTheFiveLineProperties();

	
	ControllerAMove* m_moveController;	    // �ƶ�������
    LoadingBar* m_hpBar;
    int m_iMaxHP;
//	TollgateMapLayer* _layer;
 
    CC_SYNTHESIZE(int, m_iLevel, iLevel);       // �ȼ�
    CC_SYNTHESIZE(float, m_fShowTime, fShowTime);   // �����������
    CC_SYNTHESIZE_BOOL(m_isMoveEnd, MoveEnd);	//�Ƿ�ﵽĿ�ĵ�

};

#endif