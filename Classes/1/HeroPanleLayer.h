#ifndef __HeroPanleLayer_H_
#define __HeroPanleLayer_H_
#include "cocos2d.h"
#include "Hero.h"

USING_NS_CC;

typedef enum
{
	ARROW_TOWER = 1,
	ATTACK_TOWER=2,
	MULTIDIR_TOWER,
	ANOTHER
}HeroType;


class HeroPanleLayer :public Layer{
public:
	HeroPanleLayer();
	~HeroPanleLayer();

	virtual bool init() override;
	CREATE_FUNC(HeroPanleLayer);

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	CC_SYNTHESIZE(HeroType, chooseHeroType, ChooseHeroType);

private:
	Sprite* sprite1;
	Sprite* sprite2;
	Sprite* sprite3;

};


#endif