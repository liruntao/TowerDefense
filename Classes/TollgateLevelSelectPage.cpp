#include "TollgateLevelSelectPage.h"

#include "SimpleAudioEngine.h"
#include "SceneManager.h"
#include "GlobalClient.h"
using namespace CocosDenshion;

#define LEVEL_ROW (3)
#define LEVEL_COL (2)
#define STAR_NUMBER (3)

TollgateLevelSelectPage* TollgateLevelSelectPage::create(const std::string&bgName, int level){
	TollgateLevelSelectPage* pRet = new TollgateLevelSelectPage();
	if (pRet && pRet->initLevelPage(bgName, level)){
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;//??
	}
}
bool TollgateLevelSelectPage::initLevelPage(const std::string& bgName, int level){
	if (!Node::init()){
		return false;
	}
	auto size = Director::getInstance()->getWinSize();
	auto sprite = Sprite::create(bgName);
	sprite->setPosition(Point(size.width / 2, size.height / 2));
	addChild(sprite, -2);
	//设置添加星星的宽和高
	auto starWidth = Sprite::createWithSpriteFrameName("starbg.png")->getContentSize().width;
	auto starHeight = Sprite::createWithSpriteFrameName("starbg.png")->getContentSize().height;


	Vector<MenuItem*> menuItemVector;
	auto buttonSize = Sprite::create("card_1.png")->getContentSize();
	auto gap = buttonSize.width / 4;
	auto startWidth = (size.width - LEVEL_ROW*buttonSize.width - (LEVEL_ROW - 1)*gap) / 2;
	auto startHeight = (size.height + (LEVEL_COL - 1)*gap + buttonSize.height*LEVEL_COL) / 2 - buttonSize.height;
	for (int row = 0; row != LEVEL_COL; row++){
		//log("row=%d", row);
		//log("level=%d", level);

		auto height = startHeight - (buttonSize.height + gap)*row;
		for (int col = 0; col != LEVEL_ROW; col++){
		//	log("col=%d", col);

			auto width = startWidth + (buttonSize.width + gap)*col;
			auto item = MenuItemImage::create(
				"card_1.png",
				"card_2.png",
				"card_3.png",
				CC_CALLBACK_1(TollgateLevelSelectPage::menuStartCallback, this));
			item->setAnchorPoint(Point(0, 0));
			item->setPosition(Point(width, height));
			auto tag = row*LEVEL_ROW + col + level*LEVEL_ROW*LEVEL_COL + 1;
			item->setTag(tag);
		
			log("tag=%d", row*LEVEL_ROW + col + level*LEVEL_ROW*LEVEL_COL+1);
		
			menuItemVector.pushBack(item);
			//添加每关的星星
			log("Star=%d", GlobalClient::getInstance()->getStar(tag));

			if (GlobalClient::getInstance()->getStar(tag) > 0){
				for (auto i = 0; i < GlobalClient::getInstance()->getStar(tag); ++i){		
					auto sprite = Sprite::createWithSpriteFrameName("star.png");
					sprite->setPosition(Point(width + (i - 1) * starWidth, height+starHeight*2));
					sprite->setAnchorPoint(Point(0, 0.5f));
					this->addChild(sprite, 0);

				/*	Sprite* sprite = Sprite::createWithSpriteFrameName("star.png");
					item->setAnchorPoint(Point(0, 0));
					item->setPosition(Point(width, height+i));
					this->addChild(sprite);*/
				}
			}


			//存疑
			std::string throwTollgateKey="throwTollgate";
			auto levelNum = GlobalClient::getInstance()->getValue(throwTollgateKey).asInt();
			if (levelNum < row*LEVEL_ROW + col + level*LEVEL_ROW*LEVEL_COL+1){
				item->setEnabled(false);
			}
		}
	}
	auto levelMenu = Menu::createWithArray(menuItemVector);
	levelMenu->setPosition(Point::ZERO);
	this->addChild(levelMenu, -1);

	return true;
}

void TollgateLevelSelectPage::menuStartCallback(Ref* pSender){
	auto button = (Sprite*)pSender;
	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str(),false);

	GlobalClient::getInstance()->setiCurTollgateLevel(button->getTag());
	SceneManager::getInstance()->changeScene(SceneManager::en_TollgateScene);

	//其实就是根据所选的button，来选择关卡。
	//char buffer[20] = { 0 };
	//??
	//sprintf(buffer, "levelInfo_%d.plist", button->getTag());
	//log("tag=%d", button->getTag());

	//std::string strName = buffer;
	//UserDefault::getInstance()->setStringForKey("nextLevelFile", strName);
	/*待改
	Scene* scene = Scene::create();
	auto layer = TollgateLevelInfoScene::create();
	scene->addChild(Layer);
	auto scene = TransitionFade::create(0.5, scene);
	Director::getInstance()->replaceScene(scene);*/
}
