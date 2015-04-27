/*
点击地图，弹出建造界面，使用createwithspriteframename来创建layer，先创建一个towerpos的底板，
然后再相应的在格子里创建炮塔，layer开启setSwallowTouches(true)，意思就是这个面板出来了，除非选择，否则点击其他的地方不管用（在这个layer范围内?），
通过格子的选择来返回点击的是哪一个炮塔。
但是不创建炮塔，得到heromanager里创建

我这里想在每关之前有个选择英雄和建造炮塔的界面，可以看到地形，出场怪物，在地图上编辑炮塔位置，在哪里添加什么类型的炮塔，
选择在本关可以建造的英雄（有个容器，push进去，在本关建造的时候，只能选择这个容器里的英雄，），点击确认以后，就可以保存起来了
不是每关，也可以每个阶段（该阶段地形是固定的，但是怪物和一些事件可能是不同的）
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
	touchListener->setSwallowTouches(true);//它出来以后，点击其他地方就不管用了
	touchListener->onTouchBegan = CC_CALLBACK_2(HeroPanleLayer::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HeroPanleLayer::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, sprite1);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), sprite2);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), sprite3);

	return true;
}


//以下为点击时获得tiledmap的坐标
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
