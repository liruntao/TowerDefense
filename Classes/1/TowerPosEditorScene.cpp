#include "TowerPosEditorScene.h"
#include "TowerPos.h"
#include "TowerPosEditorLayer.h"
#include "TowerPosEditorOperateLayer.h"
#include "I18N.h"
#include "EnumStrKey.h"

TowerPosEditorScene::TowerPosEditorScene() {

}

TowerPosEditorScene::~TowerPosEditorScene() {

}

Scene* TowerPosEditorScene::createScene() {
    auto scene = Scene::create();

    auto editorLayer = TowerPosEditorLayer::create();
    scene->addChild(editorLayer, 1);

    auto operLayer = TowerPosEditorOperateLayer::create(editorLayer);
    scene->addChild(operLayer, 2);

    /* ²âÊÔI18N¹¤¾ßÀà */
    log("I18N Test: %s", I18N::getInstance()->getcString(en_StrKey_Public_Confirm));
    return scene;
}

bool TowerPosEditorScene::init() {
    if (!Layer::init())
    {
        return false;
    }
    return true;
}