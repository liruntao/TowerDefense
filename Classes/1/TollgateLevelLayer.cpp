#include "TollgateLevelLayer.h"
#include "TollgateSelectScene.h"

TollgateLevelLayer::TollgateLevelLayer():
pageNode(0),curPageNode(0)
{
}
TollgateLevelLayer::~TollgateLevelLayer(){

}
bool TollgateLevelLayer::init(){
	if (!Layer::init()){
		return false;
	}
	auto touchListener = EventListenerTouchOneByOne::create();
	//可以FALSE，是否吞没所有的点击
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(TollgateLevelLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(TollgateLevelLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(TollgateLevelLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	WINDOW_WIDTH = Director::getInstance()->getWinSize().width;
	WINDOW_HEIGHT = Director::getInstance()->getWinSize().height;

	return true;
}
void TollgateLevelLayer::goToCurrNode(){
	//Point::Point()应该是 Point::Vec2(),如果不行，就修改一下。
	this->runAction(MoveTo::create(0.4f, Point::Point(-curPageNode*WINDOW_WIDTH, 0)));
	
}
void TollgateLevelLayer::addNode(Node* level){
	if (level){
		level->setContentSize(Size::Size(WINDOW_WIDTH, WINDOW_HEIGHT));
		level->setPosition(Point(WINDOW_WIDTH*pageNode, 0));
		this->addChild(level);
		pageNode++;
	}
}

bool TollgateLevelLayer::onTouchBegan(Touch *touch, Event  *event)
{
	touchDownPoint = touch->getLocation();
	touchCurPoint = touchDownPoint;
	return true;
}

void TollgateLevelLayer::onTouchMoved(Touch* touch, Event* event){
	Point touchPoint = touch->getLocation();
	auto currX = this->getPositionX() + touchPoint.x - touchCurPoint.x;
	Point posPoint = Point::Point(currX, getPositionY());
	auto dis = fabsf(touchPoint.x - touchCurPoint.x);
	if (dis >= SHORTEST_SLIDE_LENGTH){
		setPosition(posPoint);
	}
	touchCurPoint = touchPoint;
}
void TollgateLevelLayer::onTouchEnded(Touch* touch, Event* event){
	touchUpPoint = touch->getLocation();
	auto dis = touchUpPoint.getDistance(touchDownPoint);
	auto sprite1 = Sprite::createWithSpriteFrameName("page_mark1.png");
	auto width = sprite1->getContentSize().width;
	if (dis >= SHORTEST_SLIDE_LENGTH){
		int offset = getPositionX() - curPageNode*(-WINDOW_WIDTH);
		if (offset > width){
			if (curPageNode > 0){
				--curPageNode;
				Sprite* sprite = (Sprite*)TollgateSelectScene::getInstance()->getChildByTag(888);
				sprite->setPosition(Point(sprite->getPositionX() - width,sprite->getPositionY()));
			}
		}
		else if (offset < -width){
			if (curPageNode < (pageNode - 1)){
				++curPageNode;
				Sprite* sprite = (Sprite*)TollgateSelectScene::getInstance()->getChildByTag(888);
				sprite->setPosition(Point(sprite->getPositionX() + width, sprite->getPositionY()));
			}
		}
		goToCurrNode();
	}
}

