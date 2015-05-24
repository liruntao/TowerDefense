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

	//这里设定炮弹溅射范围
	auto  bulletRect = Rect((this->convertToNodeSpace(heroWorldPos)).x - this->getSprite()->getContentSize().width / 2,
		(this->convertToNodeSpace(heroWorldPos)).y - this->getSprite()->getContentSize().height / 2,100000,100000);
	for (int j = 0; j < monsterVector.size(); j++){

		auto monster = monsterVector.at(j);
		//这个monster的rect可能要改，因为添加的地方不一样，例子里是添加到maplayer上，我这里是添加到monsterlayer上
		auto monsterRect = Rect(monster->convertToNodeSpace(heroWorldPos).x - monster->getContentSize().width / 4,
			monster->convertToNodeSpace(heroWorldPos).y - monster->getContentSize().height / 4,
			monster->getContentSize().width / 2,
			monster->getContentSize().height / 2);

		if (bulletRect.intersectsRect(monsterRect)){
			//扣怪物的血，还有子弹和怪物移除还没有写。
			//应该调用bulletmanager里的函数了，
			//调用bulletmanager里的logic函数（就是一直检测是否到达目的地）里的aim->hurtme（entity里的函数，这个那个都可以用啊。）
			monster->hurtMe(this->getiAtkValue());
			//this->getSprite()->removeFromParent();
			//以后试试在这里，一旦碰撞了以后，就直接把bullet移除，而不用等到arrive以后再移除。
			//bullet->setUsed(false);
			this->removeFromParent();

			
		}
	}
	this->removeFromParent();
}
void BulletRiprap::moveEnd() {
	m_isArrive = true;



}


