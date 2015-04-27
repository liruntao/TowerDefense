/*
�����ײ��⣬��ԭ����ɾ�������ӵ���ӵ�����globalclient�
�����Ƿ��ܴ��������ӵ������������˵��
*/

#include "BulletManager.h"
#include "BulletBase.h"
#include "BulletNormal.h"
#include "GlobalClient.h"
#include "BulletMulti.h"
#include "BulletRiprap.h"

BulletManager::BulletManager() {
}

BulletManager::~BulletManager() {
	NOTIFY->removeAllObservers(this);

}

BulletManager* BulletManager::create(int iCur) {
    BulletManager* bulletMgr = new BulletManager();

    if(bulletMgr && bulletMgr->init(iCur)) {
        bulletMgr->autorelease();
    }
    else {
        CC_SAFE_DELETE(bulletMgr);
    }

    return bulletMgr;
}

bool BulletManager::init(int iCur) {
   
    //createBullets(iCur);
    return true;
}

BulletBase* BulletManager::createBullets(int iCur) {
	switch (iCur){
	case 1:{
			   m_bullet = BulletNormal::create();
			  
			   break;
	}
	case 2:{
		m_bullet = BulletMulti::create();
		break;
	}
	case 3:{
		m_bullet = BulletRiprap::create();
		break;
	}
  }
	return m_bullet;
}
