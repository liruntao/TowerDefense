#include "TowerSelectEditorPage.h"
#include "SimpleAudioEngine.h"
#include "SceneManager.h"
#include "GlobalClient.h"


using namespace CocosDenshion;
#define LEVEL_ROW (3)
#define LEVEL_COL (2)
#define STAR_NUMBER (3)

TowerSelectEditorPage::TowerSelectEditorPage(){

}
TowerSelectEditorPage::~TowerSelectEditorPage(){

}

/*TowerSelectEditorPage* TowerSelectEditorPage::create(){
	TowerSelectEditorPage* pRet = new TowerSelectEditorPage();
	if (pRet && pRet->initLevelPage()){
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;//??
	}
}*/
bool TowerSelectEditorPage::init(){
	if (!Node::init()){
		return false;
	}

//在这里添加一个点击，设置swallowtouchEs为TRUE，自然就能满足需求了。
	auto listerer = EventListenerTouchOneByOne::create();
	listerer->onTouchBegan=[](Touch* touch, Event* event){return true; };
	listerer->onTouchEnded = [](Touch* touch, Event* event){};
	listerer->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listerer, this);
	
//	auto size = Director::getInstance()->getWinSize();
	Button* exitButton = Button::create("CloseNormal.png", "CloseSelected.png");
	auto exitItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(TowerSelectEditorPage::menuCloseCallback, this));
	exitItem->setPosition(Point(0, 0));
	//？？
	auto menu = Menu::create(exitItem, NULL);
	menu->setPosition(Point(120, 100));
	this->addChild(menu, 1);
	

	auto sprite = Sprite::create("selectLevelBg1.png");
	
	sprite->setPosition(Point(0,0));
	sprite->setAnchorPoint(Point(0, 0));
	//addChild(sprite, -2);

	auto sprite1 = Sprite::create("card_1.png");
	sprite1->setPosition(Point(120, 120));
	sprite1->setAnchorPoint(Point(0, 0));
	//sprite->addChild(sprite1, 1);

	
	//获得可选择的英雄和子弹种类
	std::string canchooseherotype = "herotype";
	canChooseHeroType = GlobalClient::getInstance()->getValueVector(canchooseherotype);
	std::string canchoosebullettype = "bullettype";
	canChooseBulletType = GlobalClient::getInstance()->getValueVector(canchoosebullettype);

	ui::ScrollView* main_scrollView = ui::ScrollView::create();
	main_scrollView->setTouchEnabled(true);
	main_scrollView->setContentSize(Size(600.0f, 300.0f));
	//把这里的大小设置一下，以后好获取
	this->setContentSize(Size(150.0f, 100.0f));
	main_scrollView->setSize(Size(150, 100));
	//main_scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	main_scrollView->setDirection(SCROLLVIEW_DIR_VERTICAL);
//	main_scrollView->setInnerContainerSize(Size(1380.0f, 1000.0f));
	main_scrollView->setPosition(Point(0,0));
	//main_scrollView->setBackGroundColor(Color3B::GREEN);
//	main_scrollView->setBackGroundColorType(LAYOUT_COLOR_SOLID);

	this->addChild(main_scrollView);
	ImageView* imageView = ImageView::create();
	imageView->loadTexture("successfulBg.png");
	imageView->setSize(Size(10, 17));
	imageView->setAnchorPoint(Point(0, 0));
	imageView->setPosition(Point(0, 0));
//	main_scrollView->addChild(imageView,1);
	main_scrollView->setBackGroundImage("successfulBg.png");


	

	//添加herotype
	ui::ScrollView* heroScrollView = ui::ScrollView::create();
	//heroScrollView->setTouchEnabled(true);
	heroScrollView->setSize(Size(320, 40));
//	heroScrollView->setContentSize(Size(200.0f, 200.0f));
//	heroScrollView->setInnerContainerSize(Size(100.0f, 100.0f));
	heroScrollView->setDirection(SCROLLVIEW_DIR_HORIZONTAL);
	heroScrollView->setTouchEnabled(true);
	heroScrollView->setBounceEnabled(true);

	//总大小没有设置，

	Vector<MenuItem*> heromenuItemVector;
	auto buttonSize = Sprite::create("sprite/hero/hero_1.png")->getContentSize();
	auto gap = buttonSize.width / 4;
	int x = 1;
	for (auto i : canChooseHeroType){
		int herotype = i.asInt();
        std::string heroPath = StringUtils::format("sprite/hero/hero_%d.png", i);
		auto heroitem = MenuItemImage::create(
			heroPath,
			heroPath,
			heroPath,
			CC_CALLBACK_1(TowerSelectEditorPage::heromenuStartCallback, this));
		heroitem->setAnchorPoint(Point(0, 0));
		heroitem->setPosition(Point((x - 1) * (buttonSize.width+gap), 0));
		auto tag = x;
		heroitem->setTag(tag);
		heromenuItemVector.pushBack(heroitem);
		
		//main_scrollView->addChild(heroitem, 2);
		++x;

	}
	auto heroMenu = Menu::createWithArray(heromenuItemVector);
	heroMenu->setPosition(Point::ZERO);
	heroMenu->setAnchorPoint(Point(0, 0));
	//main_scrollView->addChild(heroMenu, 2);
	heroScrollView->addChild(heroMenu);

	heroScrollView->setPosition(Point::ZERO);
	heroScrollView->setBackGroundColor(Color3B::GREEN);
	heroScrollView->setBackGroundColorType(LAYOUT_COLOR_SOLID);
//	heroScrollView->setAnchorPoint(Point(0, 0));
	heroScrollView->setBounceEnabled(true);


	ImageView* imageView1 = ImageView::create();
	imageView1->loadTexture("HelloWorld.png");
//	imageView1->setSize(Size(10, 17));
	imageView1->setAnchorPoint(Point(0, 0));
	imageView1->setPosition(Point(0, 0));
	//heroScrollView->addChild(imageView1);
	heroScrollView->setBackGroundImage("HelloWorld.png");

	//this->addChild(heroScrollView);
	main_scrollView->addChild(heroScrollView);


	ui::ScrollView* bulletScrollView = ui::ScrollView::create();
	bulletScrollView->setContentSize(Size(1000.0f, 100.0f));
	bulletScrollView->setDirection(SCROLLVIEW_DIR_HORIZONTAL);
	bulletScrollView->setSize(Size(220, 40));
	bulletScrollView->setPosition(Point(0, 50));
	bulletScrollView->setBounceEnabled(true);
	bulletScrollView->setTouchEnabled(true);
	


	//总大小没有设置，

	Vector<MenuItem*> bulletmenuItemVector;
	//auto buttonSize = Sprite::create("sprite/hero/hero_1.png")->getContentSize();
	//auto gap = buttonSize.width / 4;
	int y = 1;
	for (auto i : canChooseBulletType){
		int herotype = i.asInt();
		//std::string bulletPath = StringUtils::format("sprite/hero/hero_%d.png", i);
		auto bulletitem = MenuItemImage::create(
			"sprite/bullet/bulletNor.png",
			"sprite/bullet/bulletArrow.png",
			"sprite/bullet/bulletRip.png",
			CC_CALLBACK_1(TowerSelectEditorPage::bulletmenuStartCallback, this));
		bulletitem->setAnchorPoint(Point(0, 0));
		bulletitem->setPosition(Point((y - 1) * (buttonSize.width + gap), 0));
		auto tag = y;
		bulletitem->setTag(tag);
		bulletmenuItemVector.pushBack(bulletitem);
		++y;

	}
	auto bulletMenu = Menu::createWithArray(bulletmenuItemVector);
	bulletMenu->setPosition(Point::ZERO);
	bulletScrollView->addChild(bulletMenu);
//	bulletScrollView->setPosition(Point(140.f, 0.0f));

	bulletScrollView->setBackGroundColor(Color3B::BLACK);
	bulletScrollView->setBackGroundColorType(LAYOUT_COLOR_SOLID);
	main_scrollView->addChild(bulletScrollView);


	return true;
}

void TowerSelectEditorPage::bulletmenuStartCallback(Ref* pSender){
	bulletType = 1;
	auto bulletbutton = (Sprite*)pSender;
	bulletType = bulletbutton->getTag();
	NOTIFY->postNotification("bulleTypeChange", (Ref*)bulletType);

	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str(), false);
}
void TowerSelectEditorPage::heromenuStartCallback(Ref* pSender){
	heroType = 1;
	auto herobutton = (Sprite*)pSender;
	heroType = herobutton->getTag();
	NOTIFY->postNotification("heroTypeChange", (Ref*)heroType);

	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str(), false);
}

//点击确定，传递出去选择的英雄种类和子弹种类
void TowerSelectEditorPage::menuCloseCallback(Ref* pSender)
{
	NOTIFY->postNotification("towerEditorDataChange", (Ref*)this);
	this->removeFromParent();
}

