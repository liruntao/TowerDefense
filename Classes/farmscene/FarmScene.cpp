#include "FarmScene.h"

#define SHOP_ITEM_LAYOUT_TAG 100
#define SEEDPANEL_TAG        888
#define HARVESTPANEL_TAG     889
#define REMOVEPANEL_TAG      890

const char* shop_textures[8] = {
	"H_864/shopItem/Item1.png",
	"H_864/shopItem/Item2.png",
	"H_864/shopItem/Item3.png",
	"H_864/shopItem/Item4.png",
	"H_864/shopItem/Item5.png",
	"H_864/shopItem/Item6.png",
	"H_864/shopItem/Item1.png",
	"H_864/shopItem/Item2.png",

};

const char* move_textures[8] = {
	"H_864/shopItem_en/Item1.png",
	"H_864/shopItem_en/Item2.png",
	"H_864/shopItem_en/Item3.png",
	"H_864/shopItem_en/Item4.png",
	"H_864/shopItem_en/Item5.png",
	"H_864/shopItem_en/Item6.png",
	"H_864/shopItem_en/Item1.png",
	"H_864/shopItem_en/Item2.png",

};

const char* shop_info[8] = {
	"土地",
	"破书",
	"土草",
	"土物",
	"破哦",
	"土的",
	"土33",
	"土44",

};
const int shop_money[8] = {
	20, 40, 60, 70, 80, 90, 100, 111,
};



FarmScene::FarmScene()
:buyTarget(NULL)
, comeOut(false)
, createTarget(false)
, deltax(0)
, deltay(0)
, longPress(false){
}

FarmScene::~FarmScene(){

}

Scene* FarmScene::createScene(){
	auto scene = Scene::create();
	auto layer = FarmScene::create();
	scene->addChild(layer);
	return scene;
}

bool FarmScene::init(){
	if (!Layer::init()){
		return false;
	}
	Size winSize = Director::getInstance()->getWinSize();

	auto closeItem = MenuItemImage::create(
		"H_864/CloseNormal.png",
		"H_864/CloseSelected.png",
		CC_CALLBACK_1(FarmScene::menuCloseCallback, this));

	closeItem->setPosition(winSize.width - closeItem->getContentSize().width / 2,
		closeItem->getContentSize().height * 12);

	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);

	mapLayer = Layer::create();
	this->addChild(mapLayer, -1);

	bgSprite = Sprite::create("2.jpg");
	bgSprite->setAnchorPoint(Point::ZERO);
	bgSprite->setPosition(Point::ZERO);
	bgOrigin = Point(Point::ZERO);
	mapLayer->addChild(bgSprite);

	auto treeSprite = Sprite::create("1.png");
	treeSprite->setAnchorPoint(Point::ZERO);
	treeSprite->setPosition(Point::ZERO);
	treeSprite->setScale(2);
	bgSprite->addChild(treeSprite, 1);

	map = TMXTiledMap::create("mymap8.tmx");
	map->setAnchorPoint(Point::ZERO);
	map->setPosition(Point::ZERO);
	bgSprite->addChild(map);

	currPos = Point(Point::ZERO);
	perPos = currPos;

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(FarmScene::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(FarmScene::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(FarmScene::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, bgSprite);

	scheduleUpdate();
	initUI();

	return true;
}

void FarmScene::initUI(){
	playerLayout = static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Ui_3/Ui_1.json"));
	addChild(playerLayout, 10);

	panel_shop = dynamic_cast<Layout*>(playerLayout->getChildByName("panel_shop"));
	auto shop_scrollView = dynamic_cast<ScrollView*>(panel_shop->getChildByName("scrollview_shop"));
	
	for (int i = 0; i < shop_scrollView->getChildren().size(); ++i){
		Layout* shop_layout = static_cast<Layout*>(shop_scrollView->getChildren().at(i));
		shop_layout->setTag(SHOP_ITEM_LAYOUT_TAG + i);

		ImageView* buy_Sprite = static_cast<ImageView*>(shop_layout->getChildByName("shopitem"));
		buy_Sprite->loadTexture(shop_textures[i]);
		
		buy_Sprite->addTouchEventListener(CC_CALLBACK_2(FarmScene::SpriteCallback, this));
		
		

		TextField* info = static_cast<TextField*>(shop_layout->getChildByName("info"));
		info->setText(shop_info[i]);

		TextField* money = static_cast<TextField*>(shop_layout->getChildByName("money_image")->getChildByName("money"));
		money->setText(std::to_string(shop_money[i]));
	}
	//这里不行就把uiwidget那个再改改
	shop_btn = dynamic_cast<Button*>(playerLayout->getChildByName("button_shop"));
	shop_btn->addTouchEventListener ( CC_CALLBACK_2(FarmScene::menuShopCallback, this));
	
	comeOut = false;
}

void FarmScene::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
	Size winSize = Director::getInstance()->getWinSize();
	if (comeOut == true)
	{
		panel_shop->runAction(EaseElasticOut::create(MoveBy::create(1, Point(-panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
		shop_btn->runAction(EaseElasticOut::create(MoveBy::create(1, Point(-panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
		comeOut = false;
	}
}

void FarmScene::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
	auto winSize = Director::getInstance()->getWinSize();
	if (touches.size() > 1)
	{
		auto point1 = touches[0]->getLocation();
		auto point2 = touches[1]->getLocation();

		/*auto currDistance = point1.distance(point2);
		distance就是下边的函数
		float Point::distance(const Point& v) const
		{
			float dx = v.x - x;
			float dy = v.y - y;

			return sqrt(dx * dx + dy * dy);
		}*/

		float dx = point1.x - point2.x;
		float dy = point1.y - point2.y;
		auto currDistance = sqrt(dx * dx + dy * dy);

		//auto prevDistance = touches[0]->getPreviousLocation().distance(touches[1]->getPreviousLocation());
		auto point3 = touches[0]->getPreviousLocation();
		auto point4 = touches[1]->getPreviousLocation();
		float dx1 = point3.x - point4.x;
		float dy1 = point3.y - point4.y;
		auto prevDistance = sqrt(dx1 * dx1 + dy1 * dy1);

		auto pointVec1 = point1 - bgOrigin;
		auto pointPoint = point2 - bgOrigin;

		auto relMidx = (pointVec1.x + pointPoint.x) / 2;
		auto relMidy = (pointVec1.y + pointPoint.y) / 2;

		auto anchorX = relMidx / bgSprite->getBoundingBox().size.width;
		auto anchorY = relMidy / bgSprite->getBoundingBox().size.height;

		auto absMidx = (point2.x + point1.x) / 2;
		auto absMidy = (point2.y + point1.y) / 2;

		if (bgOrigin.x > 0)
		{
			absMidx -= bgOrigin.x;
			bgOrigin.x = 0;
		}
		if (bgOrigin.x < -bgSprite->getBoundingBox().size.width + winSize.width)
		{
			absMidx += -bgSprite->getBoundingBox().size.width + winSize.width - bgOrigin.x;
			bgOrigin.x = -bgSprite->getBoundingBox().size.width + winSize.width;
		}
		if (bgOrigin.y > 0)
		{
			absMidy -= bgOrigin.y;
			bgOrigin.y = 0;
		}
		if (bgOrigin.y < -bgSprite->getBoundingBox().size.height + winSize.height)
		{
			absMidy += -bgSprite->getBoundingBox().size.height + winSize.height - bgOrigin.y;
			bgOrigin.y = -bgSprite->getBoundingBox().size.height + winSize.height;
		}

		bgSprite->setAnchorPoint(Point(anchorX, anchorY));
		bgSprite->setPosition(Point(absMidx, absMidy));

		auto scale = bgSprite->getScale() * (currDistance / prevDistance);
		scale = MIN(4, MAX(1, scale));
		bgSprite->setScale(scale);

		bgOrigin = Point(absMidx, absMidy) - Point(bgSprite->getBoundingBox().size.width * anchorX, bgSprite->getBoundingBox().size.height * anchorY);
	}
	else if (touches.size() == 1)
	{
		auto touch = touches[0];
		auto diff = touch->getDelta();
		auto currentPos = bgSprite->getPosition();
		auto pos = currentPos + diff;
		//代码中有一点需要注意的是，在缩放过程中，bgSprite的尺寸不断变化的，所以计算起锚点或进行边界处理时，
		//一定要用它缩放后的尺寸宽高来计算，而不能是它本身的宽高。 所以代码中计算bgSprite的尺寸我们用getBoundingBox函数来获得经过缩放和旋转之后的外框盒大小，
		//而不用getContentSize函数来获得节点原始的大小。
		
		auto bgSpriteCurrSize = bgSprite->getBoundingBox().size;

		pos.x = MIN(pos.x, bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x);
		pos.x = MAX(pos.x, -bgSpriteCurrSize.width + winSize.width + bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x);

		pos.y = MIN(pos.y, bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y);
		pos.y = MAX(pos.y, -bgSpriteCurrSize.height + winSize.height + bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y);

		bgSprite->setPosition(pos);
		Point off = pos - currentPos;
		bgOrigin += off;

	//	longPress = false;
	//	map->getLayer("tipsLayer")->removeTileAt(touchObjectPos);
	//	this->unschedule(schedule_selector(FarmScene::updatePress));
	}
}

void FarmScene::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{
	
}
void FarmScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void FarmScene::menuShopCallback(cocos2d::Ref* pSender, Widget::TouchEventType type)
{
	Size winSize = Director::getInstance()->getWinSize();
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
		  shop_btn->runAction(EaseElasticInOut::create(Sequence::create(ScaleBy::create(0.1f, 2), ScaleBy::create(0.2f, 0.5f), NULL), 0.5f));

	/*  auto seedPanel = this->getChildByTag(SEEDPANEL_TAG);
		 if (seedPanel){
		  this->removeChild(seedPanel);
		  }
										  auto removePanel = this->getChildByTag(REMOVEPANEL_TAG);
										  if (removePanel){
											  this->removeChild(removePanel);
										  }
										  auto harvestPanel = this->getChildByTag(HARVESTPANEL_TAG);
										  if (harvestPanel){
											  this->removeChild(harvestPanel);
										  }
										  for (TimingLayer* timingLayerTemp : timingVector)
										  {
											  timingLayerTemp->setVisible(false);
										  }*/
	}
		break;
	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		if (comeOut == false)
		{
			panel_shop->runAction(EaseElasticOut::create(MoveBy::create(1, Point(panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
			shop_btn->runAction(EaseElasticOut::create(MoveBy::create(1, Point(panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
			comeOut = true;
		}
		else if (comeOut == true)
		{
			panel_shop->runAction(EaseElasticOut::create(MoveBy::create(1, Point(-panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
			shop_btn->runAction(EaseElasticOut::create(MoveBy::create(1, Point(-panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
			comeOut = false;
		}
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void FarmScene::SpriteCallback(cocos2d::Ref* pSender, Widget::TouchEventType type)
{
}