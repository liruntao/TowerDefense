#include "WinScene.h"
#include "GlobalPath.h"
#include "GlobalParam.h"
#include "SceneManager.h"
#include "GlobalClient.h"

#define LEVEL_ROW (3)
#define LEVEL_COL (2)
#define STAR_NUMBER (3)

Scene* WinScene::createScene() {
    auto scene = Scene::create();

    WinScene* layer = WinScene::create();
    scene->addChild(layer);
    return scene;

	return scene;
}

bool WinScene::init() {
    if (!Layer::init()){
        return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto size = Director::getInstance()->getWinSize();
	
	auto sprite = Sprite::create("successfulBg.png");
	sprite->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(sprite, -1);		
	auto buttonSize = Sprite::create("card_1.png")->getContentSize();
	auto gap = buttonSize.width / 4;

	auto starWidth = Sprite::createWithSpriteFrameName("starbg.png")->getContentSize().width;
	auto starHeight = Sprite::createWithSpriteFrameName("starbg.png")->getContentSize().height;
	
	auto startWidth = (size.width - LEVEL_ROW*buttonSize.width - (LEVEL_ROW - 1)*gap) / 2;
	auto startHeight = (size.height + (LEVEL_COL - 1)*gap + buttonSize.height*LEVEL_COL) / 2 - buttonSize.height;


	int iCurNum = GlobalClient::getInstance()->getiCurTollgateLevel();
	int starNum = GlobalClient::getInstance()->getStar(iCurNum);
	auto startPosX = (size.width - STAR_NUMBER * starWidth) / 2;

	for (int i = 1; i <= starNum; i++)
	{
		auto sprite = Sprite::createWithSpriteFrameName("star.png");
		sprite->setPosition(Point(startPosX + (i - 1) * starWidth, size.height / 3 * 2));
		sprite->setAnchorPoint(Point(0, 0.5f));
		this->addChild(sprite, 0);
	}

	/* 添加一个标签 
	Label* winLab = Label::create("You Win!", PATH_FONT_PUBLIC, GlobalParam::PublicFontSizeLarge);
	winLab->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(winLab);*/

	/* 3秒后返回关卡选择场景 */
	this->schedule(schedule_selector(WinScene::backToTollgateSelectScene), 3.0f);

    return true;
}

void WinScene::backToTollgateSelectScene( float dt ) {
	SceneManager::getInstance()->changeScene(SceneManager::en_TollgateSelectScene);
}
