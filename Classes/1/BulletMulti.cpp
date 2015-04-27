#include "BulletMulti.h"
#include <algorithm>
#include "GlobalPath.h"
#include "GlobalClient.h"
#include "BulletNormal.h"

BulletMulti::BulletMulti(){
	m_iSpeed = SPEED_NORMAL;
	setRate(60);
	setType(2);

}
BulletMulti::~BulletMulti(){
	//NOTIFY->removeAllObservers(this);
}
bool BulletMulti::init(){
	bool bRet = false;
	do{
		Sprite* sprite = Sprite::create(PATH_BULLET_ARR);
		CC_BREAK_IF(!sprite);
		CC_BREAK_IF(!init(sprite));
		bRet = true;
	} while (0);

	return bRet;
}
BulletMulti* BulletMulti::create(Sprite* sprite){
	BulletMulti* multiBul = new BulletMulti();
	if (multiBul && multiBul->init(sprite)){
		multiBul->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(multiBul);
	}
	return multiBul;
}

bool BulletMulti::init(Sprite* sprite){
	bindSprite(sprite);
	/*NOTIFY->addObserver(this,
		callfuncO_selector(BulletMulti::recvBulletNormalType),
		"BulletType",
		NULL);*/

	return true;
}
/*void BulletMulti::recvBulletNormalType(Ref* pData){
	
	removeBullets(pData);
}*/


void BulletMulti::onLockAim(Entity* aim){
	m_isArrive = false;
	//��Ϊbullet�Ǽ��ص�bulletmanager�ϵģ����ӵ���ʾ��Ҫ��entity���ڵ��Ǹ�layer��
	//���bullet������Ҫת����Layer��������
	//�����и����ʣ�������ν��getParent���������Ҫ�Ǽ��صĶ࣬�Ƿ�Ҫ���ü���getParent������
	Point aimWorldPos = aim->getParent() -> convertToWorldSpace(aim->getPosition());
	Point dstPos = this->getParent()->convertToNodeSpace(aimWorldPos);
	
	

	createBullet6(aim);
	//����Ϊ��û��Ӱ��ʱ���ӵ��ɵ�monster���ڵ�λ���ϡ�
	//auto moveTo = MoveTo::create(0.5f, dstPos);
	//auto callFunc = CallFunc::create([&](){
	//	moveEnd();
	//});
//	auto actions = Sequence::create(moveTo, callFunc, NULL);
	//this->runAction(actions);
}
void BulletMulti::moveEnd(){
	m_isArrive = true;
}

//�������
void BulletMulti::createBullet6(Entity* aim){

	//dirTotal=10ʱ��tan��i*2*3.14/dirtotal������1����9ֵ�ֱ�Ϊ��0.011��0.022һֱ��0.099��

	/*int dirTotal = 7;

	for (int i = 0; i != dirTotal; i++)
	{
		auto curBullet = BulletMulti::create();
		GlobalClient::getInstance()->BulletVector.pushBack(curBullet);

		curBullet->setiAtkValue(this->getiAtkValue());
		this->addChild(curBullet);

		auto moveDuration = getRate();

		Point shootVector;
		shootVector.x = 0;
		shootVector.y = tan(i *0.3 * M_PI / dirTotal);
		Point normalizedShootVector;
		 if (i >= dirTotal / 2)
		{
		//	Point temp = shootVector;
			
			Point temp = Point(shootVector.x, shootVector.y - tan( 2*0.3 * M_PI / dirTotal));
			temp.normalize();
			normalizedShootVector = temp;
		}
		else
		{
			//Point x = shootVector;
			//x = x*(-1);
			//Point temp = Point(x.x, x.y + tan( 0.4 * M_PI / dirTotal));
			Point temp = shootVector;
			temp.normalize();
	    	temp = temp*(-1);
			normalizedShootVector = temp;
		}
		auto farthestDistance = Director::getInstance()->getWinSize().width-650;
	//	log("farthestDistance= %f",farthestDistance);

		Point aimWorldPos = aim->getParent()->convertToWorldSpace(aim->getPosition());
		Point dstPos = this->getParent()->convertToNodeSpace(aimWorldPos);
	
		Point overshotVector = normalizedShootVector*farthestDistance;
		Point offscreenPoint = (dstPos - overshotVector);
		//Point offscreenPoint = overshotVector;

		log("11");
	
		curBullet->runAction(Sequence::create(
			MoveTo::create(moveDuration / 100, offscreenPoint),
			//MoveBy::create(moveDuration / 10, offscreenPoint),

			CallFuncN::create(CC_CALLBACK_1(BulletMulti::removeBullet, curBullet)),
			NULL));

		curBullet = NULL;
	}*/


	int dirTotal = 7;

	for (int i = 0; i != dirTotal; i++)
	{
	auto curBullet = BulletMulti::create();
	GlobalClient::getInstance()->BulletVector.pushBack(curBullet);

	curBullet->setiAtkValue(this->getiAtkValue());
	auto moveDuration = getRate();

	//������������ʱ�������΢��һ��Ƕ�
	auto rotateVector = aim->getPosition() - this->getParent()->getPosition();
	auto rotateRadians = rotateVector.getAngle();
	auto rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians);

	//float a=i/10;
	float degree=0;
	if (i >= (dirTotal+1) / 2)
	{
	float a = (float)i;
	degree = (a-dirTotal) / dirTotal;
	
	}
	else
	{
		float a = (float)i;
		degree= a/dirTotal;
	}
	//����
	float useRadians = rotateRadians + degree;
	log("rotateRadians=%f", rotateRadians);
	log("degree=%f", degree);
	log("useRadians=%f", useRadians);
	auto useVector= ccpForAngle(useRadians);
	auto farthestDistance = Director::getInstance()->getWinSize().width-550;
	Point overshotVector = useVector * farthestDistance;
/*	Point aimWorldPos = aim->getParent()->convertToWorldSpace(aim->getPosition());
	Point dstPos = this->getParent()->convertToNodeSpace(aimWorldPos);

	curBullet->setRotation(useDegree);*/
	auto size = curBullet->getSprite()->getContentSize();
	auto physicsBody = PhysicsBody::createBox(size);
	physicsBody->setDynamic(true);
	physicsBody->setCategoryBitmask(HEROBULLET_CATEGORY_BITMASK);
	physicsBody->setCollisionBitmask(HEROBULLET_COLLISION_BITMASK);
	physicsBody->setContactTestBitmask(HEROBULLET_CONTACTTEST_BITMASK);
	//curBullet->getSprite()->setRotation(rotateDegrees);
	physicsBody->setRotationEnable(true);
	curBullet->setPhysicsBody(physicsBody);
	curBullet->setTag(BULLET_SEVERALTAG);
	//curBullet->getSprite()->setRotation(rotateDegrees);
	curBullet->setRotation(rotateDegrees);

	this->addChild(curBullet);


	curBullet->runAction(Sequence::create(
		MoveBy::create(moveDuration / 50, overshotVector),
	CallFuncN::create(CC_CALLBACK_1(BulletMulti::removeBullet, curBullet)),
	NULL));

	//curBullet = NULL;
	}
	log("");
}

void BulletMulti::removeBullet(Node* pSender)
{
	GlobalClient *instance = GlobalClient::getInstance();


	BulletBase *bullet = (BulletBase *)pSender;
	//�����һ���á�ɾ����һ������ȷ��ɾ���ľ�����ɾȥ�ģ�
	instance->BulletVector.eraseObject(bullet,false);
	bullet->getSprite()->removeFromParent();
	bullet->removeFromParent();

}

/*void BulletMulti::removeBullets(Ref* pSender)
{
	GlobalClient *instance = GlobalClient::getInstance();

	BulletBase *bullet = (BulletBase *)pSender;
	instance->BulletVector.eraseObject(bullet);
	//bullet->getSprite()->removeFromParent();
	bullet->removeFromParent();

}*/