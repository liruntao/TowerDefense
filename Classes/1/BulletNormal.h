/*
    �ļ�����    BulletNormal.h
    �衡����    ��ͨ�ӵ�
    �����ˣ�    ��ľͷ_�ӵ��� (���ͣ�http://www.benmutou.com)
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