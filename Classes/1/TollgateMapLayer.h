/*
    �ļ�����    TollgateMapLayer.h
    �衡����    �ؿ���ͼͼ��
    �����ˣ�    ��ľͷ_�ӵ��� (���ͣ�http://www.benmutou.com)

    �������ڣ�   2013.03.22
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


	//���������Ϊ������a*�㷨����Ϊa*�㷨�ǳ�ռ���ڴ棬���Լ��㲽���ʱ������tilemap�ķ����������Լ���������
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
	/* ��ǰ�ؿ� */
    int m_iCurLevel;
	//ValueMap& properties;
	void addPhysics(Point pos);
    HeroManager* m_heroMgr;
    MonsterManager* m_monsterMgr;

	bool onContactBegin(PhysicsContact& contact);
	void onContactSeperate(PhysicsContact& contact);
//	virtual void update(float dt) override; //;
    /* ��ȡ�ؿ����� */
    void loadConfig();

    void logic(float dt);

    void createEndPoint();
    void createStartPoint();

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	//�Ȱ���ײ�����ӽ�ȥ
	void CollisionDetection();

	//����ӵ�
	Vector<Node*>pointsVector;
	float offX;
	bool isTouchEnable;
	HeroPanleLayer* chooseTowerpanel;
	void addTowerChoosePanel(Point position);
	Point covertTotileCoord(Point position);
	Point covertToMatrixCoord(Point position);
	void checkAndAddTowerPanle(Point position);
	//Ϊmonster�����������
	void recvRefreshMonsterDeadBody(Ref* pData);
	Vector<Sprite*> deadMonster_vector;
	TMXObjectGroup* objects;
//	TMXObjectGroup* bodyResouceObjects;
	GlobalClient* instance;
//	void initBodyResourceVector();
	ValueVector BodyResource;
	void SlowCheck(float dt);

	//����monster��chang����ֵ��
	void ChangeMonsterTmxValue();

//	void onDraw(const kmMat4 &transform, bool transformUpdated);
//	CustomCommand _customCommand;
	//void CollisionDetection();

	Point towerPos;
//	void initPointsVector(float offX);

	

};

#endif