#ifndef __MainScene_H_
#define __MainScene_H_
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

#include "editor-support/cocostudio/CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC_EXT;
#include "TollgateSelectScene.h"
#include "SceneManager.h"


class MainScene : public Layer {
public:
	static Scene* createScene();
	virtual bool init();

	CREATE_FUNC(MainScene);

private:
	void EditorScene(Ref* target, TouchEventType type);
	void SelectScene(Ref* target, TouchEventType type);
	void ConfigureScene(Ref* target, TouchEventType type);
	void ExitGame(Ref* target, TouchEventType type);

};

#endif




