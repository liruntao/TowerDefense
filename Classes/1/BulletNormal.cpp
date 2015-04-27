#include "BulletNormal.h"
#include "ControllerSimpleMove.h"
#include "GlobalPath.h"
#include "GlobalClient.h"

//bulletnormal�б������ԣ�������Ŀ�ĵؼ��˺�Ŀ�꣬ͬ���������ٶ�Ҫ��Ƶĺܿ졣

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

bool BulletNormal::init( Sprite* sprite ) {//�̳���ʵ��Ķ�Ҫ��һ��sprite������Ļ�����������Ҳ��������ײ��⡣���ԣ�����ʲô��˼��
	bindSprite(sprite);                     //����init������create��ʲô��    

	

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
onlockaim����������Ŀ�꣬����m_isarriveΪ�٣�
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
        //��MOVto�����ˣ�ʣ�µľ�����ײ����ˡ�
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
