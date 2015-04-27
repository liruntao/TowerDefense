#include "TollgateScene.h"
#include "GlobalParam.h"
#include "TollgateMapLayer.h"
#include "TollgateDataLayer.h"

Scene* TollgateScene::createScene() {
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto layer = TollgateScene::create();
	layer->setPhyWorld(scene->getPhysicsWorld());

    TollgateScene* tgLayer = TollgateScene::create();
    TollgateMapLayer* mapLayer = TollgateMapLayer::create();
    TollgateDataLayer* dataLayer = TollgateDataLayer::create();
	
	scene->addChild(layer);
	layer->addChild(mapLayer, 1, TAG_MAP_LAYER);
	layer->addChild(tgLayer, 3);
	layer->addChild(dataLayer, 5, TAG_DATA_LAYER);

    mapLayer->initData();
    return scene;
}

bool TollgateScene::init() {
    if (!Layer::init())
    {
        return false;
    }
    return true;
}
