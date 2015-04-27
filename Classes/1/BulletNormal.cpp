#include "BulletNormal.h"
#include "ControllerSimpleMove.h"
#include "GlobalPath.h"
#include "GlobalClient.h"

//bulletnormal有必中属性，即到达目的地即伤害目标，同样的它的速度要设计的很快。

BulletNormal::BulletNormal() {
	m_iSpeed = SPEED_NORMAL;
	setType(1);
}

BulletNormal::~BulletNormal() {
	
}

BulletNormal* BulletNormal::create( Sprite* sprite ) {
	BulletNormal* bulletNor = new BulletNormal();

	if(bulletNor && bulletNor->init(sprite)) {
		bulletNor->autorelease();
	}
	else {
		CC_SAFE_DELETE(bulletNor);
	}

	return bulletNor;
}

bool BulletNormal::init( Sprite* sprite ) {//继承自实体的都要绑定一个sprite，否则的话，看不见，也不好做碰撞检测。不对，这里什么意思？
	bindSprite(sprite);                     //两个init，两个create有什么用    

	

	return true;
}

bool BulletNormal::init() {
	bool bRet = false;

	do {
		Sprite* sprite = Sprite::create(PATH_BULLET_NOR);
		CC_BREAK_IF(!sprite);

		CC_BREAK_IF(!init(sprite));

		bRet = true;
	} while (0);

	return bRet;
}

/*
onlockaim，就是锁定目标，先置m_isarrive为假，
*/

void BulletNormal::onLockAim( Entity* aim ) {
    m_isArrive = false;
	auto rotateVector = aim->getPosition() - this->getParent()->getPosition();
	auto rotateRadians = rotateVector.getAngle();
	auto rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians);
	this->getSprite()->setRotation(rotateDegrees);
	
    Point aimWorldPos = aim->getParent()->convertToWorldSpace(aim->getPosition());
   // Point dstPos = this->getParent()->convertToNodeSpace(aim->getPosition());
	Point dstPos = this->getParent()->convertToNodeSpace(aimWorldPos);

	m_aim = aim;
    auto moveTo = MoveTo::create(0.5f, dstPos);
    auto callFunc = CallFunc::create([&](){
        //就MOVto就行了，剩下的就是碰撞检测了。
		moveEnd();
    });

    auto actions = Sequence::create(moveTo, callFunc, NULL);
    this->runAction(actions);
}
void BulletNormal::removeBullet(Node* pSender)
{
	if (m_aim != NULL){
		if (m_aim->isDead()==false) {
			log("1234");
			//m_aim->hurtMe(this->getiAtkValue());
		}
	}
	
	this->removeFromParent();
	

}

void BulletNormal::moveEnd() {
    m_isArrive = true;
	//this->removeBullet(this);
	if (m_aim != NULL){
		log("1234");
//		m_aim->hurtMe(this->getiAtkValue());
	}

	this->removeFromParent();
}
