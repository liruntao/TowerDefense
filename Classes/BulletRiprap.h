#ifndef _BulletRiprap__H_
#define _BulletRiprap__H_

#include "BulletBase.h"
#include "cocos2d.h"

USING_NS_CC;

class BulletRiprap :public BulletBase{
public:
	BulletRiprap();
	~BulletRiprap();

	CREATE_FUNC(BulletRiprap);
	virtual bool init();

	static BulletRiprap* create(Sprite* sprite);
	bool init(Sprite* sprite);
	void removeBullet(Node* pSender);
	virtual void onLockAim(Entity* aim) override;

protected:
private:
	void moveEnd();
};




#endif