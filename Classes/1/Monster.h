/*
    文件名：    Monster.h
    描　述：    怪物基类
    创建人：    笨木头_钟迪龙 (博客：http://www.benmutou.com)
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
	
	

    /* 给定怪物ID，从配置文件中读取怪物数据 */
	static Monster* createFromCsvFileByID(int iMonsterID, TollgateMapLayer* layer);
    bool initFromCsvFileByID(int iMonsterID,TollgateMapLayer* layer);

	//绑定sprite
	static Monster* create(Sprite* sprite);
	bool init(Sprite* sprite);
	ControllerAMove* getMoveController();

protected:
    virtual void onDead() override;
    virtual void onBindSprite() override;
    virtual void onHurt(int iHurtValue) override;
public:
    /* 按照给定的坐标点移动 */
    void moveByPosList(Vector<PosBase*> posList);
private:
	//monster的属性VauleMap
	ValueMap theFiveLineProperties;
	void initValueMapTheFiveLineProperties();

	
	ControllerAMove* m_moveController;	    // 移动控制器
    LoadingBar* m_hpBar;
    int m_iMaxHP;
//	TollgateMapLayer* _layer;
 
    CC_SYNTHESIZE(int, m_iLevel, iLevel);       // 等级
    CC_SYNTHESIZE(float, m_fShowTime, fShowTime);   // 出场间隔：秒
    CC_SYNTHESIZE_BOOL(m_isMoveEnd, MoveEnd);	//是否达到目的地

};

#endif