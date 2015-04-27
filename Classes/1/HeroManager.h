/*
    文件名：    HeroManager.h
    描　述：    英雄管理器
    创建人：    笨木头_钟迪龙 (博客：http://www.benmutou.com)

    创建日期：   2013.05.04
*/
#ifndef _HeroManager_H_
#define _HeroManager_H_

#include "cocos2d.h"
USING_NS_CC;
#include "HeroPanleLayer.h"
#include "TowerBorder.h"
#define TOWER_POS_LAYER_LVL 5       // 塔坐标的层次
#define TOWER_BORDER_LAYER_LVL 8    // 炮台的层次
#define TOWER_LAYER_LVL 10          // 塔的层次


class PosBase;
class Monster;
class HeroManager : public Node {
public:
    HeroManager();
    ~HeroManager();
    static HeroManager* createWithLevel(int iCurLevel);

    bool initWithLevel(int iCurLevel);

    void logic(float dt, Vector<Monster*> monsterList);

	//新添加的，为了加载选择炮塔的layer。
	// bool onTouchBegan(Touch* touch, Event* event);// override,使用override声明的成员函数不能重写基类成员
//	 bool onTouchEnded(Touch* touch, Event* event) ;//override,不重写那就没必要用virtual。

	//virtual void update(float dt) override;

	//新的加了Tmxmap的create,不能用，还是c++没学好，对变量什么的了解的不深
//	传进来一个position试试，create通常设计成static时，应该是为了让别的类访问的时候方便。
	//没必要，直接点击获取position即可，哪里用得着去传进来position。
	static HeroManager* createWithLevelAndPosition(int iCurLevel,Point* position);
//创建一个全局变量试试，使用create工厂方法。
	TowerBorder* clickBorder;

private:
    /* 存放所有塔的坐标对象 */
    Vector<PosBase*> m_towerPosList;

    /* 存放所有炮台对象 */
    Vector<TowerBorder*> m_towerBorderList;

    /* 创建炮台 */
    void createTowerBorder(int iCurLevel);

    /* 给定坐标，生成塔坐标对象 */
    void createTowerPos(Point pos);

    /* 找到被单击的炮台对象 */
    TowerBorder* findClickTowerBorder(Point pos);

	///为了删除它方便，就是把创建Heropanellayer全部删掉。
	TowerBorder* border;

	Point Heroborderposition;

	//消息添加，为了在点击的时候，创建hero
	void recvTowerBorderPosition(Ref* pData);//点击的那个炮塔的位置。
	void recvTowerHeroType(Ref* pData);//炮塔种类。
	Point recvTBPosition;//炮塔坐标


	//新添加的，为了加载选择炮塔的layer。好多函数应该都没用
	Vector<Node*>pointsVector;
	float offX;
	bool isTouchEnable;
	HeroPanleLayer* chooseTowerpanel;
	void addTowerChoosePanel(Point position);
	Point covertTotileCoord(Point position);
	Point covertToMatrixCoord(Point position);
	void checkAndAddTowerPanle(Point position);
	void CollisionDetection();

	Point towerPos;
	void initPointsVector(float offX);
	void addTower();
	Hero** towerMatrix;

	const TMXTiledMap* map;
	TMXLayer* bgLayer;
	TMXObjectGroup* objects;

};

#endif