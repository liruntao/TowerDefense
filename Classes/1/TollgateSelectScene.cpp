#include "TollgateSelectScene.h"
#include "SceneManager.h"
#include "GlobalParam.h"
#include "GlobalPath.h"
#include "GlobalClient.h"

#include "TollgateLevelSelectPage.h"
#include "TollgateLevelLayer.h"
#include "SimpleAudioEngine.h"
#include "UIScene.h"
using namespace CocosDenshion;

#define LAYER_NUMBER (4)

//??怎么给做成单例了，暂时先这样吧，有问题了再说
TollgateSelectScene* TollgateSelectScene::instance;

TollgateSelectScene* TollgateSelectScene::getInstance(){
	if (instance != NULL){
		return instance;
	}
	return NULL;
}


Scene* TollgateSelectScene::createScene() {
    auto scene = Scene::create();

    TollgateSelectScene* layer = TollgateSelectScene::create();
    scene->addChild(layer);
    return scene;
}

bool TollgateSelectScene::init() {
   /* bool bRet = false;

    do {
        CC_BREAK_IF(! Layer::init());

         加载UI 
        auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("TgSelectUI/TgSelectUI_1.ExportJson");
        this->addChild(UI);
        Button* tgSelect1Btn = (Button*)Helper::seekWidgetByName(UI, "tgSelect1");
        Button* tgSelect2Btn = (Button*)Helper::seekWidgetByName(UI, "tgSelect2");
        Button* tgSelect3Btn = (Button*)Helper::seekWidgetByName(UI, "tgSelect3");

		Button* GoTOMainScene = (Button*)Helper::seekWidgetByName(UI, "GoTOMain");
		

        tgSelect1Btn->addTouchEventListener(this, toucheventselector(TollgateSelectScene::level_1));
        tgSelect2Btn->addTouchEventListener(this, toucheventselector(TollgateSelectScene::level_2));
        tgSelect3Btn->addTouchEventListener(this, toucheventselector(TollgateSelectScene::level_3));

		GoTOMainScene->addTouchEventListener(this, toucheventselector(TollgateSelectScene::GotoMainScene));
        bRet = true;
    } while (0);

    return bRet;*/


	if (!Scene::init())
	{
		return false;
	}
	/*Player初始化，暂时先这么用，以后再改。
	int levelnum = 1;
	int star = 3;
	NOTIFY->postNotification("PlayerThrowTollgateChange", (Ref*)levelnum);
	NOTIFY->postNotification("PlayerIndexChange", (Ref*)levelnum);
	NOTIFY->postNotification("PlayerStarChange", (Ref*)star);*/

	instance = this;
	Size size = Director::getInstance()->getWinSize();
	TollgateLevelLayer* scrollView = TollgateLevelLayer::create();
	for (int i = 0; i < LAYER_NUMBER; ++i){
		if (0 == i){
			auto page = TollgateLevelSelectPage::create("selectLevelBg.png", i);
			page->setTag(i);
			scrollView->addNode(page);
		}
		else if (1 == i){
			auto page = TollgateLevelSelectPage::create("selectLevelBg1.png", i);
			page->setTag(i);
			scrollView->addNode(page);
		}
		else if (2 == i){
			auto page = TollgateLevelSelectPage::create("selectLevelBg2.png", i);
			page->setTag(i);
			scrollView->addNode(page);
		}
		else if (3 == i){
			auto page = TollgateLevelSelectPage::create("selectLevelBg2.png", i);
			page->setTag(i);
			scrollView->addNode(page);
		}
	}
	Sprite* spritebg = Sprite::create("playbg.png");
	spritebg->setPosition(Point(size.width / 2, size.height / 2));
	addChild(spritebg);

	auto closeItem = MenuItemImage::create(
		"btn_back.png",
		"btn_back_press.png",
		CC_CALLBACK_1(TollgateSelectScene::menuCloseCallback, this));
	closeItem->setPosition(Point(40, 40));

	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);

	this->addChild(scrollView);

	auto selectHouse = Sprite::create("House.png");
	selectHouse->setPosition(Point(size.width / 5 * 4, size.height / 5));
	addChild(selectHouse);


	auto width = Sprite::createWithSpriteFrameName("page_mark2.png")->getContentSize().width;
	auto posX = (size.width - width * LAYER_NUMBER) / 2;
	for (int i = 0; i < LAYER_NUMBER; i++)
	{
		Sprite* sprite = Sprite::createWithSpriteFrameName("page_mark2.png");
		sprite->setPosition(Point(posX + i * width, size.height / 7));
		addChild(sprite);
	}

	Sprite* spriteMark = Sprite::createWithSpriteFrameName("page_mark1.png");
	spriteMark->setPosition(Point(posX, size.height / 7));
	spriteMark->setTag(888);
	addChild(spriteMark);

	ParticleSystem* m_emitter1 = ParticleSystemQuad::create("snow.plist");
	m_emitter1->retain();
	ParticleBatchNode *batch = ParticleBatchNode::createWithTexture(m_emitter1->getTexture());
	batch->addChild(m_emitter1);
	m_emitter1->setPosition(Point(size.width / 2, size.height));
	addChild(batch, 10);
	m_emitter1->release();

	return true;

}
void TollgateSelectScene::menuCloseCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str(), false);
	CCDirector::getInstance()->replaceScene(CCTransitionFade::create(0.5, UIScene::createScene()));
}

/*
void TollgateSelectScene::level_1(Ref* target, TouchEventType type) {
    if (type != TouchEventType::TOUCH_EVENT_ENDED){
        return;
    }
    GlobalClient::getInstance()->setiCurTollgateLevel(1);
    SceneManager::getInstance()->changeScene(SceneManager::en_TollgateScene);
}

void TollgateSelectScene::level_2(Ref* target, TouchEventType type) {
    if (type != TouchEventType::TOUCH_EVENT_ENDED){
        return;
    }
    GlobalClient::getInstance()->setiCurTollgateLevel(2);
    SceneManager::getInstance()->changeScene(SceneManager::en_TollgateScene);
}

void TollgateSelectScene::level_3(Ref* target, TouchEventType type) {
    if (type != TouchEventType::TOUCH_EVENT_ENDED){
        return;
    }
    GlobalClient::getInstance()->setiCurTollgateLevel(3);
    SceneManager::getInstance()->changeScene(SceneManager::en_TollgateScene);
}

void TollgateSelectScene::GotoMainScene(Ref* target, TouchEventType type){
	if (type != TouchEventType::TOUCH_EVENT_ENDED){
		return;
	}
	SceneManager::getInstance()->changeScene(SceneManager::en_MainScene);
}*/
