#ifndef _LoadingResScene_H__
#define _LoadingResScene_H__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class LoadingResScene :public Layer{
public:
	LoadingResScene();
	~LoadingResScene();
	CREATE_FUNC(LoadingResScene);
	static Scene* createScene();
	virtual bool init();

	ProgressTimer* addProgress();
	void loadResources();
	//void transitionScene();
	void loadingCallBack(cocos2d::Texture2D* texture);
	void logic(float dt);
	void transitionScene();
private:
	ProgressTimer* progressBar;
	int numberOfLoadedRes;
	int totalOfLoadedRes;
	
};

#endif