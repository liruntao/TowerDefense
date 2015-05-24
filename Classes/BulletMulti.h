#ifndef __BulletMulti_H__
#define __BulletMulti_H__

#include "cocos2d.h"
USING_NS_CC;

#include "BulletBase.h"

class BulletMulti :public BulletBase{
public:
	BulletMulti();
	~BulletMulti();

	CREATE_FUNC(BulletMulti);
	virtual bool init();

	static BulletMulti* create(Sprite* sprite);
	void createBullet6(Entity* aim);
	void removeBullet(Node* pSender);
	void removeBullets(Ref* pSender);
	bool init(Sprite* sprite);
	virtual void onLockAim(Entity* aim) override;
	void recvBulletNormalType(Ref* pData);
protected:

private:
	void moveEnd();

};




#endif