#include "SceneManager.h"
#include "TowerPosEditorScene.h"
#include "TollgateScene.h"
#include "TollgateSelectScene.h"
#include "WinScene.h"
#include "GameOverScene.h"
#include "MainScene.h"

SceneManager* SceneManager::mSceneManager = NULL;

SceneManager* SceneManager::getInstance() {
    if(mSceneManager == NULL) {
        mSceneManager = new SceneManager();
        if(mSceneManager && mSceneManager->init()) {
            mSceneManager->autorelease();
            mSceneManager->retain();
        }
        else {
            CC_SAFE_DELETE(mSceneManager);
            mSceneManager = NULL;
        }
    }

    return mSceneManager;
}

bool SceneManager::init() {

    return true;
}

void SceneManager::changeScene( EnumSceneType enScenType ) {
    Scene* pScene = NULL;

    switch (enScenType) {
    case en_TollgateScene: /* �ؿ����� */
        pScene = TollgateScene::createScene();
        break;
    case en_TollgateEditorScene:    /* �ؿ��༭������ */
        pScene = TowerPosEditorScene::createScene();
        break;
    case en_TollgateSelectScene:    /* �ؿ�ѡ�񳡾� */
        pScene = TollgateSelectScene::createScene();
        break;
	case en_WinScene:		/* ʤ������ */
        pScene = WinScene::createScene();
		break;
	case en_GameOverScene:	/* ��Ϸ�������� */
        pScene = GameOverScene::createScene();
		break;
	case en_MainScene:
		pScene = MainScene::createScene();/*�����泡��*/
		break;
    }

    if(pScene == NULL) {
        return;
    }

    Director* pDirector = Director::getInstance();
    Scene* curScene = pDirector->getRunningScene();
    if(curScene == NULL) {
        pDirector->runWithScene(pScene);
    }
    else {
        pDirector->replaceScene(pScene);
    }
}
