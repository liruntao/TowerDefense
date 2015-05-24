#ifndef __FarmScene_H__
#define __FarmScene_H__

#include "cocos2d.h"
#include "cocostudio\CCSGUIReader.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;


typedef enum
{
	GROUND = 1,
	GROUND_CROP = 2,
	CROP_HARVEST,
	OTHER,

}TileType;

class FarmScene :public Layer{
public:
	FarmScene();
	~FarmScene();

	static cocos2d::Scene* createScene();

	virtual bool init();
	void updatePress(float dt);

	void menuCloseCallback(Ref* pSender);
	void menuShopCallback(Ref* pSender, Widget::TouchEventType type);
	void SpriteCallback(Ref* pSender, Widget::TouchEventType type);
	

	virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	void initUI();

	CREATE_FUNC(FarmScene);

private:
	Layer* mapLayer;
	Layer* shopLayer;
	Menu* shopMenu;
	Sprite* shopMenubg;
	bool comeOut;
	Layout* playerLayout;

	Layout* panel_shop;
	Button* shop_btn;
	Sprite* buyTarget;
	bool createTarget;

	Sprite* bgSprite;
	Point bgOrigin;

	double deltax;
	double deltay;

	TMXTiledMap* map;
	bool canBliud;
	Point currPos;
	Point perPos;
	bool longPress;
	Point touchObjectPos;
	TileType tileType;
	//Vector<timingLayer*>timingVector;
	//void createTimingLayer(CropsType type);

	Point convertTotileCoord(Point position);
	Point convertToScreenCoord(Point position);

	void moveCheck(Point position, int tag);
	void updateRipeCrop();
	void checkTileType();

	void checkAndCreate(Point pos);
};


#endif