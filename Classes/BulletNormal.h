/*
    文件名：    BulletNormal.h
    描　述：    普通子弹
    创建人：    笨木头_钟迪龙 (博客：http://www.benmutou.com)
*/
#ifndef _BulletNormal_H_
#define _BulletNormal_H_

#include "BulletBase.h"

class BulletNormal : public BulletBase {
public:
	BulletNormal();
	~BulletNormal();

    CREATE_FUNC(BulletNormal);
    virtual bool init();

	static BulletNormal* create(Sprite* sprite);
	bool init(Sprite* sprite);
	void removeBullet(Node* pSender);
	virtual void onLockAim(Entity* aim) override;
	//Entity* m_aim;
	
protected:
private:
    void moveEnd();
};

#endif