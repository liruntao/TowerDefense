/*
    文件名：    GameOverScene.h
    描　述：    游戏结束场景
    创建人：    笨木头_钟迪龙 (博客：http://www.benmutou.com)
*/
#ifndef _GameOverScene_H_
#define _GameOverScene_H_

#include "cocos2d.h"
USING_NS_CC;

class GameOverScene : public Layer {
public:
    static Scene* createScene();
	CREATE_FUNC(GameOverScene);
	virtual bool init();

private:
	void backToTollgateSelectScene(float dt);
};

#endif