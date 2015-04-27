#ifndef _TowerSelectEditorPage_H_
#define _TowerSelectEditorPage_H_

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class TowerSelectEditorPage : public Node{
public:
	TowerSelectEditorPage();
	~TowerSelectEditorPage();

	
	//bool initLevelPage();
	//static TowerSelectEditorPage* create();
	CREATE_FUNC(TowerSelectEditorPage);
	virtual bool init();

	void bulletmenuStartCallback(Ref* pSender);
	void heromenuStartCallback(Ref* pSender);

	void menuCloseCallback(Ref* pSender);


private:
	ValueVector canChooseHeroType;
	ValueVector canChooseBulletType;
	int bulletType;
	int heroType;
};



#endif