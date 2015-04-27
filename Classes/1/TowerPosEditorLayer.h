
#ifndef _TowerPosEditorLayer_H_
#define _TowerPosEditorLayer_H_

#include "cocos2d.h"
#include "EnumPosType.h"
#include "TowerBorder.h"
#include "HeroPanleLayer.h"
USING_NS_CC;

#define MAX_LEVEL = 5;

class PosBase;
class HeroManager;

class TowerPosEditorLayer : public Layer {
public:
    TowerPosEditorLayer();
    ~TowerPosEditorLayer();

    CREATE_FUNC(TowerPosEditorLayer);
    virtual bool init();

    /* 把所有坐标对象生成到plist配置文件 */
    void outputPosToPlistFile();

    /* 切换编辑模式 */
    void changeMode();

    /* 切换到下一关卡 */
    int nextLvl();

    /* 切换到上一关卡 */
    int preLvl();

    /* 获取怪物坐标列表 */
    Vector<PosBase*> getMonsterPosList();
private:
  //简单编辑地形
	bool isWallAtTileCoord(const Point &tileCoord) const;
	bool isValidTileCoord(const Point& tileCoord) const;
	bool isPropAtTileCoordForLayer(const char *prop, const Point &tileCoord, TMXLayer *layer) const;
	Value propertiesAtTileCoord(const Point &tileCoord, TMXLayer *layer);
	Point tileCoordForPosition(const Point &position) const;
	Point positionForTileCoord(const Point &tileCoord) const;



	void recvRefreshHeroType(Ref* pData);
	void recvRefreshBulletType(Ref* pData);
	void recvRefreshTowerEditorData(Ref* pData);
	void recvRefreshHeroAndBulletType(Ref* pData);
	void recvDeletTowerBord(Ref* pData);

	int heroType;
	int bulletType;
	HeroManager* m_heroMgr;
	float offX;
	TowerBorder* clickBorder;
	TowerBorder* findClickTowerBorder(Point pos);
	Vector<TowerBorder*> m_towerBorderList;
	HeroPanleLayer* chooseTowerpanel;
	void initconfig();
	void addTower(Point pos);


	Vector<PosBase*> m_towerPosList;   /* 存放所有塔的坐标对象 */

    int m_iCurLevel;                    /* 当前关卡 */

	/* 把指定坐标对象生成到plist配置文件 */
    void outputPosToPlistFile(Vector<PosBase*> posList, const char* sFilePath);
	void outputPosToPlistFile_hero(Vector<PosBase*> posList, const char* sFilePath);

	/* 编辑hero和bullet */
//	void editHeroAndBullet(Point pos);

	/* 根据坐标找到已经存在的塔坐标对象 */
    PosBase* findExistTowerPos(Point pos);

    /* 给定坐标，生成塔坐标对象 */
    void createTowerPos(Point pos);

    /* 给定塔坐标对象，删除塔坐标对象 */
    void deleteTowerPos(PosBase* existPos);

    /* 删除所有的坐标对象 */
    void deleteAllPos();

    /* 读取配置文件 */
    void loadConfigFile();

    /* 当前模式 */
    EnumPosType m_enMode;


    /* 编辑怪物坐标 */
    void editMonsterPos(Point pos);

    /* 存放所有怪物的坐标对象 */
    Vector<PosBase*> m_monsterPosList;

    /* 根据坐标找到已经存在的怪物坐标对象 */
    PosBase* findExistMonsterPos(Point pos);

    /* 给定坐标，生成怪物坐标对象 */
    void createMonsterPos(Point pos);

    /* 给定怪物坐标对象，删除怪物坐标对象 */
    void deleteMonsterPos(PosBase* existPos);

	//添加tmxtilemap
	TMXTiledMap* map;
	TMXLayer* bgLayer;
	TMXObjectGroup* objects;

};

#endif