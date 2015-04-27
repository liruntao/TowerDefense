/*
    文件名：    WinScene.h
    描　述：    胜利场景
    创建人：    笨木头_钟迪龙 (博客：http://www.benmutou.com)
*/
#ifndef _WinScene_H_
#define _WinScene_H_

#include "cocos2d.h"
USING_NS_CC;

class WinScene : public Layer {
public:
	static Scene* createScene();
	CREATE_FUNC(WinScene);
	virtual bool init();

private:
	void backToTollgateSelectScene(float dt);
};

#endif