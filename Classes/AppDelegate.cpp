#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "TowerPosEditorScene.h"
#include "SceneManager.h"
#include "MainScene.h"
#include "LoadingResScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("TowerDefence");
       // glview->setFrameSize(1024, 800);
        director->setOpenGLView(glview);

    }

    /* 游戏设计大小
    glview->setDesignResolutionSize(800, 480, ResolutionPolicy::SHOW_ALL); */

	//glview->setDesignResolutionSize(480.0f, 320.0f, ResolutionPolicy::FIXED_HEIGHT);
/*分辨率自适配*/
	glview->setDesignResolutionSize(800.0f, 480.0f, ResolutionPolicy::SHOW_ALL);
	std::vector<std::string> searchPath;
	searchPath.push_back("height_864");
	searchPath.push_back("H_864");
	CCFileUtils::getInstance()->setSearchPaths(searchPath);
	
	//director->setContentScaleFactor(864.0f / 320.0f);
	director->setContentScaleFactor(864.0f / 480.0f);

	


    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    /* 调用场景管理器切换场景 */
   // SceneManager::getInstance()->changeScene(SceneManager::en_TollgateSelectScene);

	//auto scene = MainScene::createScene();
	auto scene = LoadingResScene::createScene();
	 director->runWithScene(scene);

//     auto scene = TowerPosEditorScene::createScene();
//     director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
