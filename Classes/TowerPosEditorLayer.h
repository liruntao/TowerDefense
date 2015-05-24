
#ifndef _TowerPosEditorLayer_H_
#define _TowerPosEditorLayer_H_

#include "cocos2d.h"
#include "EnumPosType.h"
#include "TowerBorder.h"
#include "HeroPanleLayer.h"
#include "Repair.h"
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

    /* ����������������ɵ�plist�����ļ� */
    void outputPosToPlistFile();

    /* �л��༭ģʽ */
    void changeMode();

    /* �л�����һ�ؿ� */
    int nextLvl();

    /* �л�����һ�ؿ� */
    int preLvl();

    /* ��ȡ���������б� */
    Vector<PosBase*> getMonsterPosList();

	virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	void initUI();
	void createRepairLayer(Point pos);

private:

	ValueVector canChooseHeroType;
	ValueVector canChooseBulletType;
	Button* selectType_btn;
	Button* hideSelectType_btn;
	ui::ScrollView* main_scrollView;
	Layer* uiLayer;
	Sprite* buyTarget;
	Point bgOrigin;
	bool canBuild;
	float offX;
	Point perPos;
	Point curPos;
	Vector<Repair*> repairVector;

	void menuHide(cocos2d::Ref* pSender, TouchEventType type);
	void menuShow(cocos2d::Ref* pSender, TouchEventType type);
	void heroCallBack(cocos2d::Ref* pSender, TouchEventType type);
	void moveCheck(Point pos, int heroType);
	Point tileCoordForPosition(const Point &position) const;
	Point positionForTileCoord(const Point &tileCoord) const;
	void createTowerPos(int heroType,int bulletType,Point pos);

  //�򵥱༭����
	bool isWallAtTileCoord(const Point &tileCoord) const;
	bool isValidTileCoord(const Point& tileCoord) const;
	bool isPropAtTileCoordForLayer(const char *prop, const Point &tileCoord, TMXLayer *layer) const;
	Value propertiesAtTileCoord(const Point &tileCoord, TMXLayer *layer);
//	Point tileCoordForPosition(const Point &position) const;
//	Point positionForTileCoord(const Point &tileCoord) const;



	void recvRefreshHeroType(Ref* pData);
	void recvRefreshBulletType(Ref* pData);
	void recvRefreshTowerEditorData(Ref* pData);
	void recvRefreshHeroAndBulletType(Ref* pData);
	void recvDeletTowerBord(Ref* pData);
	void recvRepairTowerBord(Ref* pData);

	int heroType;
	int bulletType;
	HeroManager* m_heroMgr;

	TowerBorder* clickBorder;
	TowerBorder* findClickTowerBorder(Point pos);
	Vector<TowerBorder*> m_towerBorderList;
	HeroPanleLayer* chooseTowerpanel;
	void initconfig();
	void addTower(Point pos);


	Vector<PosBase*> m_towerPosList;   /* ������������������ */

    int m_iCurLevel;                    /* ��ǰ�ؿ� */

	/* ��ָ������������ɵ�plist�����ļ� */
    void outputPosToPlistFile(Vector<PosBase*> posList, const char* sFilePath);
	void outputPosToPlistFile_hero(Vector<PosBase*> posList, const char* sFilePath);

	/* �༭hero��bullet */
//	void editHeroAndBullet(Point pos);

	/* ���������ҵ��Ѿ����ڵ���������� */
    PosBase* findExistTowerPos(Point pos);

    /* �������꣬������������� */
    void createTowerPos(Point pos);

    /* �������������ɾ����������� */
    void deleteTowerPos(PosBase* existPos);

    /* ɾ�����е�������� */
    void deleteAllPos();

    /* ��ȡ�����ļ� */
    void loadConfigFile();

    /* ��ǰģʽ */
    EnumPosType m_enMode;


    /* �༭�������� */
    void editMonsterPos(Point pos);

    /* ������й����������� */
    Vector<PosBase*> m_monsterPosList;

    /* ���������ҵ��Ѿ����ڵĹ���������� */
    PosBase* findExistMonsterPos(Point pos);

    /* �������꣬���ɹ���������� */
    void createMonsterPos(Point pos);

    /* ���������������ɾ������������� */
    void deleteMonsterPos(PosBase* existPos);

	//���tmxtilemap
	TMXTiledMap* map;
	TMXLayer* bgLayer;
	TMXObjectGroup* objects;

};

#endif