#include "BulletRiprap.h"
#include "ControllerSimpleMove.h"
#include "GlobalPath.h"
#include "GlobalClient.h"

BulletRiprap::BulletRiprap() {
	m_iSpeed = SPEED_NORMAL;
	setType(3);
}

BulletRiprap::~BulletRiprap() {

}

BulletRiprap* BulletRiprap::create(Sprite* sprite) {
	BulletRiprap* bulletNor = new BulletRiprap();

	if (bulletNor && bulletNor->init(sprite)) {
		bulletNor->autorelease();
	}
	else {
		CC_SAFE_DELETE(bulletNor);
	}

	return bulletNor;
}

bool BulletRiprap::init(Sprite* sprite) {
	bindSprite(sprite);                     

	return true;
}

bool BulletRiprap::init() {
	bool bRet = false;

	do {
		Sprite* sprite = Sprite::create(PATH_BULLET_RIPRAP);
		CC_BREAK_IF(!sprite);

		CC_BREAK_IF(!init(sprite));

		bRet = true;
	} while (0);

	return bRet;
}



void BulletRiprap::onLockAim(Entity* aim) {
	m_isArrive = false;

	Point aimWorldPos = aim->getParent()->convertToWorldSpace(aim->getPosition());
	Point dstPos = this->getParent()->convertToNodeSpace(aimWorldPos);


	this->runAction(Sequence::create(MoveTo::create(0.5f, dstPos),
		CallFuncN::create(CC_CALLBACK_1(BulletRiprap::removeBullet, this)),
		NULL));

}
void BulletRiprap::removeBullet(Node* pSender)
{
	BulletBase *bullet = (BulletBase *)pSender;
	auto monsterVector = GlobalClient::getInstance()->MonsterVector;

	Point heroWorldPos = this->getParent()->getParent()->getParent()->getParent()->convertToWorldSpace(this->getParent()->getParent()->getParent()->getPosition());

	//�����趨�ڵ����䷶Χ
	auto  bulletRect = Rect((this->convertToNodeSpace(heroWorldPos)).x - this->getSprite()->getContentSize().width / 2,
		(this->convertToNodeSpace(heroWorldPos)).y - this->getSprite()->getContentSize().height / 2,100000,100000);
	for (int j = 0; j < monsterVector.size(); j++){

		auto monster = monsterVector.at(j);
		//���monster��rect����Ҫ�ģ���Ϊ��ӵĵط���һ��������������ӵ�maplayer�ϣ�����������ӵ�monsterlayer��
		auto monsterRect = Rect(monster->convertToNodeSpace(heroWorldPos).x - monster->getContentSize().width / 4,
			monster->convertToNodeSpace(heroWorldPos).y - monster->getContentSize().height / 4,
			monster->getContentSize().width / 2,
			monster->getContentSize().height / 2);

		if (bulletRect.intersectsRect(monsterRect)){
			//�۹����Ѫ�������ӵ��͹����Ƴ���û��д��
			//Ӧ�õ���bulletmanager��ĺ����ˣ�
			//����bulletmanager���logic����������һֱ����Ƿ񵽴�Ŀ�ĵأ����aim->hurtme��entity��ĺ���������Ǹ��������ð�����
			monster->hurtMe(this->getiAtkValue());
			//this->getSprite()->removeFromParent();
			//�Ժ����������һ����ײ���Ժ󣬾�ֱ�Ӱ�bullet�Ƴ��������õȵ�arrive�Ժ����Ƴ���
			//bullet->setUsed(false);
			this->removeFromParent();

			
		}
	}
	this->removeFromParent();
}
void BulletRiprap::moveEnd() {
	m_isArrive = true;



}


