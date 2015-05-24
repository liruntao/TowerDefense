/*
    文件名：    TollgateMapLayer.h
    描　述：    关卡地图图层
    创建人：    笨木头_钟迪龙 (博客：http://www.benmutou.com)

    创建日期：   2013.03.22
*/
#ifndef _TollgateMapLayer_H_
#define _TollgateMapLayer_H_

#include "cocos2d.h"
#include "HeroPanleLayer.h"
#include "Hero.h"
USING_NS_CC;
#include "GlobalClient.h"
#include "TollgateScene.h"

class HeroManager;
class MonsterManager;
class GlobalClient;
class TollgateMapLayer : public Layer {
public:
    TollgateMapLayer();
    ~TollgateMapLayer();

    CREATE_FUNC(TollgateMapLayer);
    virtual bool init();

	//virtual void update(float dt) override;
	//virtual void onTouchEnded(Touch* touch, Event* event) override;

	TMXTiledMap* map;
	TMXLayer* bgLayer;
	TMXLayer* _meta;
	TMXLayer* _propertiLayer;


	//以下添加是为了增加a*算法。因为a*算法非常占用内存，所以计算步骤的时候是以tilemap的方格来计算以减少运算量
	Point tileCoordForPosition(const Point &position) const;
	Point positionForTileCoord(const Point &tileCoord) const;

	bool isWallAtTileCoord(const Point &tileCoord) const;
	bool isValidTileCoord(const Point& tileCoord) const;
	bool isPropAtTileCoordForLayer(const char *prop, const Point &tileCoord, TMXLayer *layer) const;
	Value propertiesAtTileCoord( const Point &tileCoord, TMXLayer *layer) ;
	ValueMap& getpropertiesValueMapAtTileCoord(const Point &tileCoord, TMXLayer *layer);

	void setPropAtTileCoordForLayer(const char *prop, const Point &tileCoord, TMXLayer *layer) const;
	
	PointArray* walkableAdjacentTilesCoordForTileCoord(const Point&tileCoord) const;


    void initData();
private:
	TollgateScene* _scene;
	std::unordered_map<int, Node*> _mouses;
	/* 当前关卡 */
    int m_iCurLevel;
	//ValueMap& properties;
	void addPhysics(Point pos);
    HeroManager* m_heroMgr;
    MonsterManager* m_monsterMgr;

	bool onContactBegin(PhysicsContact& contact);
	void onContactSeperate(PhysicsContact& contact);
//	virtual void update(float dt) override; //;
    /* 读取关卡配置 */
    void loadConfig();

    void logic(float dt);

    void createEndPoint();
    void createStartPoint();

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	//先把碰撞检测添加进去
	void CollisionDetection();

	//新添加的
	Vector<Node*>pointsVector;
	float offX;
	bool isTouchEnable;
	HeroPanleLayer* chooseTowerpanel;
	void addTowerChoosePanel(Point position);
	Point covertTotileCoord(Point position);
	Point covertToMatrixCoord(Point position);
	void checkAndAddTowerPanle(Point position);
	//为monster死亡后的设置
	void recvRefreshMonsterDeadBody(Ref* pData);
	Vector<Sprite*> deadMonster_vector;
	TMXObjectGroup* objects;
//	TMXObjectGroup* bodyResouceObjects;
	GlobalClient* instance;
//	void initBodyResourceVector();
	ValueVector BodyResource;
	void SlowCheck(float dt);

	//更改monster的chang属性值。
	void ChangeMonsterTmxValue();

//	void onDraw(const kmMat4 &transform, bool transformUpdated);
//	CustomCommand _customCommand;
	//void CollisionDetection();

	Point towerPos;
//	void initPointsVector(float offX);

	

};

#endif