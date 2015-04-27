
#ifndef _TollgateSelectScene_H_
#define _TollgateSelectScene_H_

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

class TollgateSelectScene : public Scene {
public:
    static Scene* createScene();
    virtual bool init();

    CREATE_FUNC(TollgateSelectScene);

	void menuCloseCallback(Ref* pSender);
	static TollgateSelectScene* getInstance();

private:
    void level_1(Ref* target, TouchEventType type);
    void level_2(Ref* target, TouchEventType type);
    void level_3(Ref* target, TouchEventType type);

	void GotoMainScene(Ref* target, TouchEventType type);

	static TollgateSelectScene* instance;
};

#endif