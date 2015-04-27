#include "MainScene.h"
#define LAYER_NUMBER (3)

Scene* MainScene::createScene() {
	auto scene = Scene::create();

	MainScene* layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainScene::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!Layer::init());
		bgOrigin = Point::ZERO;
		/* 加载UI */
		auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("UI/MainScene_1.ExportJson");
		this->addChild(UI);

		Button* Editorbtn = (Button*)Helper::seekWidgetByName(UI, "EditorMap");
		Button* Selectbtn = (Button*)Helper::seekWidgetByName(UI, "SelectMap");
		Button* Configurebtn = (Button*)Helper::seekWidgetByName(UI, "Configure");
		Button* Exitbtn = (Button*)Helper::seekWidgetByName(UI, "Exit");

		Editorbtn->addTouchEventListener(this, toucheventselector(MainScene::EditorScene));
		Selectbtn->addTouchEventListener(this, toucheventselector(MainScene::SelectScene));
		Configurebtn->addTouchEventListener(this, toucheventselector(MainScene::ConfigureScene));
		Exitbtn->addTouchEventListener(this, toucheventselector(MainScene::ExitGame));

		//下雪
		Size size = Director::getInstance()->getWinSize();
		ParticleSystem* m_emitter1 = ParticleSystemQuad::create("snow.plist");
		m_emitter1->retain();
		ParticleBatchNode *batch = ParticleBatchNode::createWithTexture(m_emitter1->getTexture());
		batch->addChild(m_emitter1);
		m_emitter1->setPosition(Point(size.width / 2, size.height));
		addChild(batch, 10);
		m_emitter1->release();

		


		/*Button* tgSelect1Btn = (Button*)Helper::seekWidgetByName(UI, "tgSelect1Btn");
		Button* tgSelect2Btn = (Button*)Helper::seekWidgetByName(UI, "tgSelect2Btn");
		Button* tgSelect3Btn = (Button*)Helper::seekWidgetByName(UI, "tgSelect3Btn");

		tgSelect1Btn->addTouchEventListener(this, toucheventselector(TollgateSelectScene::level_1));
		tgSelect2Btn->addTouchEventListener(this, toucheventselector(TollgateSelectScene::level_2));
		tgSelect3Btn->addTouchEventListener(this, toucheventselector(TollgateSelectScene::level_3));*/

		bRet = true;
	} while (0);

	return bRet;
}

void MainScene::EditorScene(Ref*, TouchEventType type){
	//这里就是说只有在点击end以后才会执行操作，以避免误操作
	if (type != TouchEventType::TOUCH_EVENT_ENDED){
		return;
	}
	SceneManager::getInstance()->changeScene(SceneManager::en_TollgateEditorScene);
}
void MainScene::SelectScene(Ref*, TouchEventType type){
	if (type != TouchEventType::TOUCH_EVENT_ENDED){
		return;
	}
	SceneManager::getInstance()->changeScene(SceneManager::en_TollgateSelectScene);
}
void MainScene::ConfigureScene(Ref*, TouchEventType type){
	if (type != TouchEventType::TOUCH_EVENT_ENDED){
		return;
	}
	return;//将来调用配置layer.
}
void MainScene::ExitGame(Ref*, TouchEventType type){
	if (type != TouchEventType::TOUCH_EVENT_ENDED){
		return;
	}
	return;//结束游戏
}

