/*
�����ͼ������������棬ʹ��createwithspriteframename������layer���ȴ���һ��towerpos�ĵװ壬
Ȼ������Ӧ���ڸ����ﴴ��������layer����setSwallowTouches(true)����˼��������������ˣ�����ѡ�񣬷����������ĵط������ã������layer��Χ��?����
ͨ�����ӵ�ѡ�������ص��������һ��������
���ǲ������������õ�heromanager�ﴴ��

����������ÿ��֮ǰ�и�ѡ��Ӣ�ۺͽ��������Ľ��棬���Կ������Σ���������ڵ�ͼ�ϱ༭����λ�ã����������ʲô���͵�������
ѡ���ڱ��ؿ��Խ����Ӣ�ۣ��и�������push��ȥ���ڱ��ؽ����ʱ��ֻ��ѡ������������Ӣ�ۣ��������ȷ���Ժ󣬾Ϳ��Ա���������
����ÿ�أ�Ҳ����ÿ���׶Σ��ý׶ε����ǹ̶��ģ����ǹ����һЩ�¼������ǲ�ͬ�ģ�
*/

#include "HeroPanleLayer.h"
#include "GlobalDefine.h"


HeroPanleLayer::HeroPanleLayer(){

}
HeroPanleLayer::~HeroPanleLayer(){

}

bool HeroPanleLayer::init(){
	if (!Layer::init())
	{
		return false;
	}

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Play.plist");

	auto sprite = Sprite::createWithSpriteFrameName("towerPos.png");
	sprite->setPosition(Point(0, 0));
	this->addChild(sprite);

	sprite1 = Sprite::createWithSpriteFrameName("ArrowTower1.png");
	sprite1->setAnchorPoint(Point(0.5f, 0));
	sprite1->setPosition(Point(-sprite->getContentSize().width, sprite->getContentSize().height / 2));
	this->addChild(sprite1);

	sprite2 = Sprite::createWithSpriteFrameName("AttackTower1.png");
	sprite2->setAnchorPoint(Point(0.5f, 0));
	sprite2->setPosition(Point(0, sprite->getContentSize().height / 2));
	this->addChild(sprite2);

	sprite3 = Sprite::createWithSpriteFrameName("MultiDirTower1.png");
	sprite3->setAnchorPoint(Point(0.5f, 0));
	sprite3->setPosition(Point(sprite->getContentSize().width, sprite->getContentSize().height / 2));
	this->addChild(sprite3);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);//�������Ժ󣬵�������ط��Ͳ�������
	touchListener->onTouchBegan = CC_CALLBACK_2(HeroPanleLayer::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HeroPanleLayer::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, sprite1);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), sprite2);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), sprite3);

	return true;
}


//����Ϊ���ʱ���tiledmap������
bool HeroPanleLayer::onTouchBegan(Touch* touch, Event* event){
	auto target = static_cast<Sprite*>(event->getCurrentTarget());

	Point locationInNode = target->convertTouchToNodeSpace(touch);
	Size size = target->getContentSize();
	Rect rect = Rect(0, 0, size.width, size.height);

	if (rect.containsPoint(locationInNode))
	{
		target->setOpacity(180);
		return true;
	}
	return false;
}

void HeroPanleLayer::onTouchEnded(Touch* touch, Event* event){
	auto target = static_cast<Sprite*>(event->getCurrentTarget());

	if (target == sprite1)
	{
		chooseHeroType = ARROW_TOWER;
	}
	else if(target==sprite2){
		chooseHeroType = ATTACK_TOWER;
	}
	else if (target == sprite3){
		chooseHeroType = MULTIDIR_TOWER;
	}
	else{
		chooseHeroType = ANOTHER;
	}

	int herotype = chooseHeroType;
	//NOTIFY->postNotification("MagicChange", (Ref*)chooseHeroType);
	NOTIFY->postNotification("HeroType", (Ref*)herotype);

}
