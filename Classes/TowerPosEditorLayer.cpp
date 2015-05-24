/*��������Ƶ�ʱ�������⣬������һ��λ����������button���������ʱ���������button���������������ʾ��
����������Ƶģ�һ��button��visible���ó�true��һ�����ó�false���������ʱ����������visible����任�����������ʾ�ĸ��档
����������addtoucheventlistener�������ֲ���ɾ�������������ʹ����һ����ͨ���ַ�������һ����zorder����Ϊ9����һ������Ϊ10����
������button�Ǹ���ʾ��ʱ�򣬾Ͱ��Ǹ���zorderֵ��ߣ�������ʵ���˹��ܡ�
*/


#include "TowerPosEditorLayer.h"
#include "TowerPos.h"
#include "PosBase.h"
#include "PosLoadUtil.h"
#include "MonsterPos.h"
#include "TowerSelectEditorPage.h"
#include "GlobalDefine.h"
#include "GlobalClient.h"
#include "HeroManager.h"
#include "extensions\cocos-ext.h"
#include "HeroPanleLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC_EXT;
using namespace CocosDenshion;


#define HOME_LAYER_LVL 3  // ���ݵĲ��
#define HERO_LAYER_LVL 10    // Ӣ�۵Ĳ��
#define MONSTER_LAYER_LVL 15    // ����Ĳ��
#define BULLET_LAYER_LVL 20  // �ӵ��Ĳ��
#define TOWER_LAYER_LVL 10 
#define MAP_WIDTH (16)//map�Ŀ�͸ߣ����Ե���
#define MAP_HEIGHT (9)
#define HEROTAG 1000

TowerPosEditorLayer::TowerPosEditorLayer()
:buyTarget(NULL),
canBuild(false)
{
   
	GlobalClient::getInstance()->setiCurTollgateLevel(1);
	m_enMode = enTowerPos;
	chooseTowerpanel = NULL;
	heroType = 1;
	bulletType = 1;
	
}

TowerPosEditorLayer::~TowerPosEditorLayer() {
	NOTIFY->removeAllObservers(this);
}

bool TowerPosEditorLayer::init() {
    if (!Layer::init())
    {
        return false;
    }
//	initconfig();

	m_iCurLevel = GlobalClient::getInstance()->getiCurTollgateLevel();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Size winSize = Director::getInstance()->getWinSize();
	
	bgOrigin = Point(Point::ZERO);
	curPos = Point(Point::ZERO);
	perPos = curPos;


	//m_heroMgr = HeroManager::createWithLevel(m_iCurLevel);
	//this->addChild(m_heroMgr, HERO_LAYER_LVL);
	/* ��ӱ������� */
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
		StringUtils::format("music/tollgate_%d.mp3", m_iCurLevel).c_str(), true);
	
	std::string sTowerPosPath = StringUtils::format("tollgate/towerPos_level_%d.plist", m_iCurLevel);
	Vector<PosBase*> posList = PosLoadUtil::getInstance()->loadPosWithFile(
		sTowerPosPath.c_str(), enTowerPos, this, 10, false);//����һ��Vector<PosBase*>posList�������������еĵ������
	m_towerPosList.pushBack(posList);//

	/**/
	for (auto tPos : m_towerPosList) {
		TowerBorder* border = TowerBorder::create();
		border->upgrade();
		border->upgrade();
		border->setPosition(tPos->getPos());
		this->addChild(border);
		//���Ӣ�ۼ��ӵ�
		int heroType = tPos->getHeroType();
		int bulletType = tPos->getBulletType();
		//Hero* hero = Hero::createFromCsvFileByID(heroType, bulletType);
		
		Hero* hero = Hero::createForSelect(heroType, bulletType);
		hero->setPosition(tPos->getPos());
		this->addChild(hero, TOWER_LAYER_LVL);
		border->bindHero(hero);
		m_towerBorderList.pushBack(border);
	}
	
	clickBorder = TowerBorder::create();
	this->addChild(clickBorder);
    clickBorder = NULL;

	NOTIFY->addObserver(this,
		callfuncO_selector(TowerPosEditorLayer::recvRefreshHeroType),
		"heroTypeChange",
		NULL); 
	NOTIFY->addObserver(this,
		callfuncO_selector(TowerPosEditorLayer::recvRefreshBulletType),
		"bulleTypeChange",
		NULL);
	NOTIFY->addObserver(this,
		callfuncO_selector(TowerPosEditorLayer::recvRefreshTowerEditorData),
		"towerEditorDataChange",
		NULL);
//�༭hero��bullet�Լ�ɾ��towerbord
	NOTIFY->addObserver(this,
		callfuncO_selector(TowerPosEditorLayer::recvRefreshHeroAndBulletType),
		"HeroAndBulletTypeChange",
		NULL);
	NOTIFY->addObserver(this,
		callfuncO_selector(TowerPosEditorLayer::recvDeletTowerBord),
		"DeletTowerBord",
		NULL);
	NOTIFY->addObserver(this,
		callfuncO_selector(TowerPosEditorLayer::recvRepairTowerBord),
		"RepairTowerBord",
		NULL);
   /* ��ȡ�����ļ� */
    loadConfigFile();
	initUI();
    /* ���������¼� 
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event){
		this->removeChild(chooseTowerpanel);
		chooseTowerpanel = NULL;
		if (clickBorder != NULL){
			clickBorder->deleteOprBtns();
		}
		
		return true;
    };
    listener->onTouchEnded = [&](Touch* touch, Event* event){
        Point pos = Director::getInstance()->convertToGL(touch->getLocationInView());

		clickBorder = findClickTowerBorder(pos);
        /// �жϵ�ǰ�༭����ģʽ�����в�ͬ�Ĳ��� 
		if (clickBorder != NULL){
       clickBorder->showTowerOprBtns();
		}
		else{
			
			Point tilePos = this->tileCoordForPosition(pos);

			if (!this->isValidTileCoord(tilePos) || this->isWallAtTileCoord(tilePos)){
			
				log("you can't go there forever!");
				return;
			}
			else
			{			
			 switch (m_enMode) {
        case enTowerPos:
           // editTowerPos(pos);
			createTowerPos(pos);
            break;
        case enMonsterPos:
            editMonsterPos(pos);
            break;
			}
			}
		}

    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);*/

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(TowerPosEditorLayer::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(TowerPosEditorLayer::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(TowerPosEditorLayer::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,map);

 
    return true;
}
void TowerPosEditorLayer::initconfig(){
	std::string sTowerPosPath = StringUtils::format("tollgate/towerPos_level_%d.plist", m_iCurLevel);
	Vector<PosBase*> posList = PosLoadUtil::getInstance()->loadPosWithFile(
		sTowerPosPath.c_str(), enTowerPos, this, 10, false);
	m_towerPosList.pushBack(posList);
	outputPosToPlistFile();
}

void TowerPosEditorLayer::initUI(){

	//��ÿ�ѡ���Ӣ�ۺ��ӵ�����
	std::string canchooseherotype = "herotype";
	canChooseHeroType = GlobalClient::getInstance()->getValueVector(canchooseherotype);
	std::string canchoosebullettype = "bullettype";
	canChooseBulletType = GlobalClient::getInstance()->getValueVector(canchoosebullettype);

	selectType_btn = Button::create("btn_back.png");
	selectType_btn->addTouchEventListener(this, toucheventselector(TowerPosEditorLayer::menuHide, this));

	hideSelectType_btn = Button::create("btn_back_press.png");
	hideSelectType_btn->setVisible(false);
	hideSelectType_btn->addTouchEventListener(this, toucheventselector(TowerPosEditorLayer::menuShow, this));

	Size selectBtn_size = selectType_btn->getContentSize();

	uiLayer = Layer::create();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	uiLayer->setContentSize(Size(visibleSize.width-selectBtn_size.width-40,selectBtn_size.height+20));
	uiLayer->setPosition(Point(selectBtn_size.width + 20, 0));
	this->addChild(uiLayer, 999);



	main_scrollView = ui::ScrollView::create();
//	main_scrollView->setTouchEnabled(true);
	main_scrollView->setContentSize(Size(600.0f, 300.0f));
	//������Ĵ�С����һ�£��Ժ�û�ȡ
//	this->setContentSize(Size(150.0f, 100.0f));
	main_scrollView->setSize(Size(150, 100));
	main_scrollView->setDirection(SCROLLVIEW_DIR_VERTICAL);
	main_scrollView->setPosition(Point(0, 0));

	uiLayer->addChild(main_scrollView);
	ImageView* imageView = ImageView::create();
	imageView->loadTexture("successfulBg.png");
	imageView->setSize(Size(10, 17));
	imageView->setAnchorPoint(Point(0, 0));
	imageView->setPosition(Point(0, 0));
	//	main_scrollView->addChild(imageView,1);
	main_scrollView->setBackGroundImage("successfulBg.png");

	//���herotype
	ui::ScrollView* heroScrollView = ui::ScrollView::create();
	//heroScrollView->setTouchEnabled(true);
	heroScrollView->setSize(Size(320, 40));
	//	heroScrollView->setContentSize(Size(200.0f, 200.0f));
	//	heroScrollView->setInnerContainerSize(Size(100.0f, 100.0f));
	heroScrollView->setDirection(SCROLLVIEW_DIR_HORIZONTAL);
	//heroScrollView->setTouchEnabled(true);
	heroScrollView->setBounceEnabled(true);
	//�ܴ�Сû�����ã�
	//��������׵Ļ�������������1���ǲ���menu��������button���޷Ǿ��Ǵ���1�����һ�Σ�2������menu
	//�����ڵ��ʱ��Ҫ����һ�������������������һ��layer��Ȼ�����Ǹ�layer���ƶ�������鷳�ɣ���
	Vector<MenuItem*> heromenuItemVector;
	auto buttonSize = Sprite::create("sprite/hero/hero_1.png")->getContentSize();
	auto gap = buttonSize.width / 4;
	int x = 1;
	for (auto i : canChooseHeroType){
		int herotype = i.asInt();
		std::string heroPath = StringUtils::format("sprite/hero/hero_%d.png", i);
		/*auto heroitem = MenuItemImage::create(
			heroPath,
			heroPath,
			heroPath,
			toucheventselector(TowerPosEditorLayer::heroCallBack, this));
			//CC_CALLBACK_2(TowerPosEditorLayer::heroCallBack, this));

		//MenuItem* heroitem = MenuItem::create(heroPath);
		//heroitem->addTouchEventListener(this, toucheventselector(TowerPosEditorLayer::heroCallBack, this));*/

		Button* heroitem=Button::create(heroPath);
		
		heroitem->setTag(HEROTAG + herotype);
		heroitem->addTouchEventListener(this, toucheventselector(TowerPosEditorLayer::heroCallBack, this));
		hideSelectType_btn->addTouchEventListener(this, toucheventselector(TowerPosEditorLayer::menuShow, this));

		heroitem->setAnchorPoint(Point(0, 0));
		heroitem->setPosition(Point((x - 1) * (buttonSize.width + gap), 0));
		//auto tag = x;
		//heroitem->setTag(tag);
	//	heromenuItemVector.pushBack(heroitem);
		heroScrollView->addChild(heroitem);
		//main_scrollView->addChild(heroitem, 2);
		++x;
	}
	/*auto heroMenu = Menu::createWithArray(heromenuItemVector);
	
	heroMenu->setPosition(Point::ZERO);
	heroMenu->setAnchorPoint(Point(0, 0));*/
	//main_scrollView->addChild(heroMenu, 2);
	//heroScrollView->addChild(heroMenu);
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


	hideSelectType_btn->setZOrder(9);
	selectType_btn->setZOrder(10);
	uiLayer->addChild(hideSelectType_btn);
	uiLayer->addChild(selectType_btn);
}

void TowerPosEditorLayer::menuHide(cocos2d::Ref* pSender, TouchEventType type)
{
	Size winSize = Director::getInstance()->getWinSize();
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
	{

	}
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
	{

	}
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
	main_scrollView->runAction(EaseElasticOut::create(MoveBy::create(1, Point(0,-main_scrollView->getContentSize().height / 3 * 2)), 0.5f));
	selectType_btn->setVisible(false);
	selectType_btn->setZOrder(9);
	hideSelectType_btn->setZOrder(10);
	hideSelectType_btn->setVisible(true);
	//	selectType_btn->removeFromParent();
//	hideSelectType_btn->addTouchEventListener(this, toucheventselector(TowerPosEditorLayer::menuShow, this));
//	uiLayer->addChild(hideSelectType_btn);
	}
	
		break;
	default:
		break;
	}
}
void TowerPosEditorLayer::menuShow(cocos2d::Ref* pSender, TouchEventType type)
{
	Size winSize = Director::getInstance()->getWinSize();
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
	{

	}
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
	{

	}
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
	main_scrollView->runAction(EaseElasticOut::create(MoveBy::create(1, Point(0, main_scrollView->getContentSize().height / 3 * 2)), 0.5f));
	//hideSelectType_btn->removeFromParent();
	hideSelectType_btn->setVisible(false);
	selectType_btn->setVisible(true);
	selectType_btn->setZOrder(10);
	hideSelectType_btn->setZOrder(9);
	}
		break;
	default:
		break;
	}
}

void TowerPosEditorLayer::heroCallBack(cocos2d::Ref* pSender, TouchEventType type)
{
	Size winSize = Director::getInstance()->getWinSize();
	Widget* widget = static_cast<Widget*>(pSender);
	auto heroType = widget->getTag() - HEROTAG;
	
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
	{
	widget->runAction(EaseElasticInOut::create(ScaleTo::create(0.1f, 1.5), 0.2f));
	}
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
	{
	if (buyTarget == NULL){
		
		std::string heroPath = StringUtils::format("sprite/hero/hero_%d.png", heroType);
		buyTarget = Sprite::create(heroPath);
	//	buyTarget->setAnchorPoint(Point(0.5f, 0.27f));
		
		map->addChild(buyTarget, 10);
		
	 }
	else
	{
		Point pos;
		pos.x = widget->getTouchMovePos().x+offX;
		pos.y = widget->getTouchMovePos().y-40;
		//pos.x = widget->getTouchMovePos().x - bgOrigin.x;
		//pos.y = widget->getTouchMovePos().y - bgOrigin.y;
		this->moveCheck(pos, heroType);
	}
	}
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	if (buyTarget != NULL){
	buyTarget->removeFromParent();
	buyTarget = NULL;
						 }
	canBuild = false;
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		if (canBuild == true){
		//	auto endPos = Point(widget->getTouchEndPos().x + offX, widget->getTouchEndPos().y);
			auto endPos = widget->getTouchEndPos();
			map->getLayer("bg")->setTileGID(20, tileCoordForPosition(endPos));
			int heroType = widget->getTag() - HEROTAG;
			int bulletType = heroType;
			this->createTowerPos(heroType, bulletType, endPos);

			canBuild = false;
		}
		else{
			auto endPos = Point(widget->getTouchEndPos().x + offX, widget->getTouchEndPos().y - 40);
			auto coord = this->tileCoordForPosition(endPos);
			auto screenPos = this->positionForTileCoord(coord);
			auto tips = Sprite::create("tip.png");
			tips -> setPosition(screenPos);
			map->addChild(tips);
			tips->runAction(Sequence::create(DelayTime::create(0.1f),
				CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, tips)),
				NULL));
		}
		if (buyTarget != NULL){
			buyTarget->removeFromParent();
			buyTarget = NULL;
		}
		map->getLayer("Meta")->removeTileAt(perPos);
		map->getLayer("Meta")->removeTileAt(curPos);
	default:
		break;
	}
}

void TowerPosEditorLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
	this->removeChild(chooseTowerpanel);
	chooseTowerpanel = NULL;
	if (clickBorder != NULL){
		clickBorder->deleteOprBtns();
	}
	Size winSize = Director::getInstance()->getWinSize();
//	for (Repair* repairTemp : repairVector){
//		repairTemp->setVisible(false);
//	}
	if (touches.size() == 1)
	{
		auto touch = touches[0];
		auto screenPos = touch->getLocation();
		auto mapSize = map->getMapSize();
		Point pos;
		pos.x = screenPos.x;
		pos.y = screenPos.y;
		auto tilePos = this->positionForTileCoord(pos);

	}
}
void TowerPosEditorLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{

}
void TowerPosEditorLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
	
	Size winSize = Director::getInstance()->getWinSize();
	if (touches.size() == 1){
		auto touch = touches[0];
	Point pos = Director::getInstance()->convertToGL(touch->getLocationInView());

	clickBorder = findClickTowerBorder(pos);
	/// �жϵ�ǰ�༭����ģʽ�����в�ͬ�Ĳ��� 
	if (clickBorder != NULL){
		clickBorder->showTowerOprBtns();
	}
	}
}

void TowerPosEditorLayer::createRepairLayer(Point pos)
{

}

//���ﻻһ��˼·���ܲ��ܽ��춼����һ��ͼ�꣬���ܽ����ֻ�����ѱ���ͼ���˾Ϳ��ԣ���������Ǹ�gid��ֵ������ܽ���gid����50��
//���ܽ��죬gid����49
void TowerPosEditorLayer::moveCheck(Point pos, int heroType)
{
	auto mapSize = map->getMapSize();
	auto tilePos = this->tileCoordForPosition(pos);
	canBuild = false;
	auto moveGid = 49;
	perPos = curPos;
//	std::string heroPath = StringUtils::format("sprite/hero/hero_%d.png", heroType);
//	buyTarget = Sprite::create(heroPath);
	//Sprite* heroBuild = Sprite::create(heroPath);
	//std::string noHeroPath = StringUtils::format("sprite/hero/hero_no_%d.png", heroType);
//	Sprite* heroNoBuild = Sprite::create(noHeroPath);
	if (tilePos.x >= 0 && tilePos.x <= mapSize.width - 1 && tilePos.y >= 0 && tilePos.y <= mapSize.height - 1)
	{
		curPos = tilePos;
		int gid = map->getLayer("bg")->getTileGIDAt(tilePos);
		if (gid == 0)
		{
			moveGid = 50;
			canBuild = true;
		}
		else
		{
			moveGid = 49;
			canBuild = false;
		}
		auto screenPos = this->positionForTileCoord(tilePos);
		buyTarget->setPosition(screenPos);
		if (perPos != curPos)
		{
			//����ƶ���ĳ��Ƭ�����ϣ����½Ǿͻ���ʾ���ӵ����꣬�Լ���ʹ�õ���Ƭ�زĵ�GID������GID���������ܣ���
			//ÿһ����ͼ��ֻ����һ����Ƭ�ز���ɣ����������⡣
			/*
			6����Ƭ��ȫ�ֱ�ʶGID

��Cocos��Ϸ�У�ÿһ����Ƭ�زĶ���һ��ȫ��Ψһ��ʶGID������Ƭ��GID���Ǳ�ʾ����Ƭ��ʹ�õ����ĸ�GID��ͼ���زġ������������С���ᵽ���Ƿ�ͼ��

GID�ļ�����1��ʼ����˳���ţ�һֱ��ŵ�ͼ���������������ͼ��tileͼ����Դ�е� ID = 0 ��ͼ���� GID = 1���Դ����ơ��� tileͼ����Դ�����һ�� ID = 47 ��ͼ���Ӧ��GID = 48��

Ȼ����ڵڶ���metaͼ����Դ�е� ID = 0 ��ͼ�飬��Ӧ�� GID = 49�����ǵģ����������ȥ������     */
			
			map->getLayer("Meta")->removeTileAt(perPos);
			map->getLayer("Meta")->setTileGID(moveGid,curPos);
			
		}
	}
	else
	{
		buyTarget->setPosition(pos);
		moveGid = 49;
		map->getLayer("Meta")->removeTileAt(perPos);
		canBuild = false;
	}

	log("moveGid=%d", moveGid);
}
//�������Ҫ�ǲ����ˣ���ȥ�ģ������˷ֱ���Ӧ���ǲ��Եġ�
Point TowerPosEditorLayer::tileCoordForPosition(const Point &position) const
{
	int x = (position.x + offX) / map->getContentSize().width*map->getMapSize().width;
	//	int x = (position.x) / map->getContentSize().width*map->getMapSize().width;

	int y = map->getMapSize().height - position.y / map->getContentSize().height*map->getMapSize().height;

	/*û���÷ֱ��ʵ�
	int x = position.x / map->getTileSize().width;
	int y = ((map->getMapSize().height*map->getTileSize().height) - position.y) / map->getTileSize().height;*/
	return Point(x, y);
}
Point TowerPosEditorLayer::positionForTileCoord(const Point &tileCoord)const
{
	int x = tileCoord.x / map->getMapSize().width*map->getContentSize().width - offX;
	int y = (map->getMapSize().height - tileCoord.y) / map->getMapSize().height*map->getContentSize().height;

	/*û���÷ֱ��ʵ�
	int x = (tileCoord.x*map->getTileSize().width) + map->getTileSize().width / 2;
	int y = (map->getMapSize().height*map->getTileSize().height) -
	(tileCoord.y*map->getTileSize().height) - map->getTileSize().height / 2;*/
	return Point(x, y);
}
/*�༭Ӣ�ۺ��ӵ�
void TowerPosEditorLayer::editHeroAndBullet(Point pos) {
	
	PosBase* existPos = findExistTowerPos(pos);
	log("Pos.x=%f,Pos.y=%f", pos.x, pos.y);

	if(existPos != NULL) {
		TowerSelectEditorPage* EditorPage = TowerSelectEditorPage::create();
		EditorPage->setAnchorPoint(Point::ZERO);
		EditorPage->setPosition(pos);
		//EditorPage->setAnchorPoint(Point(1, 1));
		this->addChild(EditorPage, 50);
		//deleteTowerPos(existPos);
	}
	
}*/





PosBase* TowerPosEditorLayer::findExistTowerPos(Point pos) {
    for (auto basePos : m_towerPosList){
        if (basePos->isClickMe(pos)) {
            return basePos;
        }
	}

	return NULL;
}

void TowerPosEditorLayer::createTowerPos( Point pos ) {
  
	
	Hero* hero = Hero::createForSelect(heroType, bulletType);
	auto  buildHeroSize = Rect(pos.x,pos.y,
		hero->getSprite()->getContentSize().width,
		hero->getSprite()->getContentSize().height);
	if (m_towerBorderList.empty()){
		addTower(pos);
	}
	else
	{	
		int num = 0;
	for (auto presentdeTower : m_towerBorderList){
		auto presentdeTowerSize = Rect(presentdeTower->getPosition().x, presentdeTower->getPosition().y,
			presentdeTower->getContentSize().width, presentdeTower->getContentSize().height);
		if (buildHeroSize.intersectsRect(presentdeTowerSize)){
			num++;
	
			break;
		}
	}
		if (num == 0){
			addTower(pos);
		}
	}
	
}
void TowerPosEditorLayer::createTowerPos(int heroType, int bulletType, Point pos) {

	Hero* hero = Hero::createForSelect(heroType, bulletType);
	TowerPos* tPos = TowerPos::create(pos, heroType, bulletType, true);
	this->addChild(tPos, 10);
	TowerBorder* create_border = TowerBorder::create();
	//�鷳�����������upgrade������Ϊ�˼�����sprite��
	create_border->upgrade();
	create_border->upgrade();
	create_border->setPosition(tPos->getPos());
	this->addChild(create_border);
	

	//Hero* hero = Hero::createFromCsvFileByID(heroType, bulletType);
	hero->setPosition(tPos->getPos());
	this->addChild(hero, TOWER_LAYER_LVL);
	create_border->bindHero(hero);

	m_towerBorderList.pushBack(create_border);
	m_towerPosList.pushBack(tPos);



}
void TowerPosEditorLayer::deleteTowerPos(PosBase* existPos) {
    this->removeChild(existPos);
    m_towerPosList.eraseObject(existPos);
}

void TowerPosEditorLayer::deleteAllPos() {
    this->removeAllChildrenWithCleanup(true);
    m_towerPosList.clear();
    m_monsterPosList.clear();
}

void TowerPosEditorLayer::outputPosToPlistFile() {
    /* �����̨���������ļ� */
    std::string sTowerPosPath = StringUtils::format("tollgate/towerPos_level_%d.plist", m_iCurLevel);
    outputPosToPlistFile_hero(m_towerPosList, sTowerPosPath.c_str());

    /* ����������������ļ�,���������˵Ӧ�ò��ã�������ʱ���������Ժ��ٸ� */
    std::string sMonsterPosPath = StringUtils::format("tollgate/monsterPos_level_%d.plist", m_iCurLevel);
    outputPosToPlistFile(m_monsterPosList, sMonsterPosPath.c_str());
}

void TowerPosEditorLayer::outputPosToPlistFile_hero(Vector<PosBase*> posList, const char* sFilePath) {
    
	//ValueMap fileDataMap = FileUtils::getInstance()->getValueMapFromFile(sFilePath);
	ValueMap fileDataMap;
    /* �������� */
   
    for (auto posBase : posList) {
        ValueMap data;
        data["x"] = posBase->getPos().x;
        data["y"] = posBase->getPos().y;
		data["bullettype"] = posBase->getBulletType();
		data["herotype"] = posBase->getHeroType();
		//fileDataMap[Value(posBase->getPos().x).asString() + Value(posBase->getPos().y).asString()];
        //fileDataMap[StringUtils::toString(index)] = Value(data);
		fileDataMap[StringUtils::toString(posBase->getPos().x) + StringUtils::toString(posBase->getPos().y)] = Value(data);

        
    }
    FileUtils::getInstance()->writeToFile(fileDataMap, sFilePath);
}
void TowerPosEditorLayer::outputPosToPlistFile(Vector<PosBase*> posList, const char* sFilePath) {

	//ValueMap fileDataMap = FileUtils::getInstance()->getValueMapFromFile(sFilePath);
	ValueMap fileDataMap;
	/* �������� */

	for (auto posBase : posList) {
		ValueMap data;
		data["x"] = posBase->getPos().x;
		data["y"] = posBase->getPos().y;
		
		fileDataMap[StringUtils::toString(posBase->getPos().x) + StringUtils::toString(posBase->getPos().y)] = Value(data);


	}
	FileUtils::getInstance()->writeToFile(fileDataMap, sFilePath);
}

void TowerPosEditorLayer::loadConfigFile() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
	Size winSize = Director::getInstance()->getWinSize();

	std::string sBG = StringUtils::format("tollgate/level_%d.tmx", m_iCurLevel);
	map = TMXTiledMap::create(sBG.c_str());
	bgLayer = map->getLayer("bg");
	objects = map->getObjectGroup("obj");

	offX = (map->getContentSize().width - winSize.width) / 2;//�������꣬ʹ�����λ����tilemap��λ����ͬ��

	bgLayer->setAnchorPoint(Point(0.5f, 0.5f));
	bgLayer->setPosition(Point(winSize.width / 2-offX, winSize.height / 2));

	//����UI


	this->addChild(map, -1);


    /* ������������� 
    std::string sTowerPosPath = StringUtils::format("tollgate/towerPos_level_%d.plist", m_iCurLevel);
    Vector<PosBase*> posList = PosLoadUtil::getInstance()->loadPosWithFile(sTowerPosPath.c_str(), enTowerPos, this, 10, true);
    m_towerPosList.pushBack(posList);*/

    /* ���ع���������� */
    std::string sMonsterPosPath = StringUtils::format("tollgate/monsterPos_level_%d.plist", m_iCurLevel);
   Vector<PosBase*> posList = PosLoadUtil::getInstance()->loadPosWithFile(sMonsterPosPath.c_str(), enMonsterPos, this, 10, true);
    m_monsterPosList.pushBack(posList);
}

void TowerPosEditorLayer::editMonsterPos(Point pos) {
    /* ����������Ѿ����ڵĹ������������ɾ����������󣬷��򴴽���������� */
    PosBase* existPos = findExistMonsterPos(pos);
    if (existPos != NULL) {
        deleteMonsterPos(existPos);
    }
    else {
        createMonsterPos(pos);
    }
}

PosBase* TowerPosEditorLayer::findExistMonsterPos(Point pos) {
    for (auto basePos : m_monsterPosList){
        if (basePos->isClickMe(pos)) {
            return basePos;
        }
    }
    return NULL;
}

void TowerPosEditorLayer::createMonsterPos(Point pos) {
    MonsterPos* mPos = MonsterPos::create(pos, true);
    this->addChild(mPos, 10);
    m_monsterPosList.pushBack(mPos);
}

void TowerPosEditorLayer::deleteMonsterPos(PosBase* existPos) {
 
	
	
	this->removeChild(existPos);
    m_monsterPosList.eraseObject(existPos);
}

void TowerPosEditorLayer::changeMode(){
    if (m_enMode == enMonsterPos) {
        m_enMode = enTowerPos;
    }
    else {
        m_enMode = enMonsterPos;
    }
}

int TowerPosEditorLayer::nextLvl() {
    deleteAllPos();		/* ɾ����ǰ���е�������� */
    m_iCurLevel++;		/* �ؿ�������1 */
    loadConfigFile();	/* ���¶�ȡ�����ļ� */

    return m_iCurLevel;
}

int TowerPosEditorLayer::preLvl() {
    deleteAllPos();		/* ɾ����ǰ���е�������� */
    m_iCurLevel--;		/* �ؿ�������1 */
    loadConfigFile();	/* ���¶�ȡ�����ļ� */

    return m_iCurLevel;
}

Vector<PosBase*> TowerPosEditorLayer::getMonsterPosList() {
    return m_monsterPosList;
}

void TowerPosEditorLayer::recvRefreshHeroType(Ref* pData){
	heroType = (int)pData;
	
}
void TowerPosEditorLayer::recvRefreshBulletType(Ref* pData){
	bulletType = (int)pData;
	
}
//ò�Ʋ���Ҫ�ϱ�����Ҳ�ܳɹ�����this����������
void TowerPosEditorLayer::recvRefreshTowerEditorData(Ref* pData){

	TowerPos* m = (TowerPos*)pData;
	Point pos = m->getPosition();
	log("Pos.x=%f,Pos.y=%f", pos.x, pos.y);

	PosBase* existPos = findExistTowerPos(pos);
	log("existPos.x=%f,existPos.y=%f", existPos->getPos().x, existPos->getPos().y);
	TowerBorder* editor_Border = findClickTowerBorder(pos);
	Hero* delete_hero = editor_Border->getHero();

	int m_heroType = existPos->getHeroType();
	int m_bulletType = existPos->getBulletType();
	if (heroType != 1)
	{
	m_heroType = heroType;
	}
	if (bulletType != 1)
	{
	m_bulletType = bulletType;
	}
	heroType = 1;
	bulletType = 1;

	if ( (m_heroType== 1) && (m_bulletType == 1)){
		return;
	}	
		existPos->setHeroType(m_heroType);
		existPos->setBulletType(m_bulletType);	
	//�޸���ʾ�������ӵ�
	
	this->removeChild(delete_hero);
	Hero* hero = Hero::createForSelect(m_heroType, m_bulletType);
	hero->setPosition(pos);
	this->addChild(hero, TOWER_LAYER_LVL);
	editor_Border->bindHero(hero);

}

void TowerPosEditorLayer::recvRefreshHeroAndBulletType(Ref* pData){
	TowerBorder* cur_towerbord = (TowerBorder*)pData;

	Point pos = cur_towerbord->getPosition();
	PosBase* existPos = findExistTowerPos(pos);
	log("Pos.x=%f,Pos.y=%f", pos.x, pos.y);

	if (existPos != NULL) {
		TowerSelectEditorPage* EditorPage = TowerSelectEditorPage::create();
		auto pageSize = EditorPage->getContentSize();
		auto visibleSize = Director::getInstance()->getVisibleSize();
		if ((pos.x + pageSize.width > visibleSize.width)&&(pos.y+pageSize.height<visibleSize.height)){
			EditorPage->setAnchorPoint(Point(1, 0));
		}
		else if ((pos.x + pageSize.width>visibleSize.width) && (pos.y + pageSize.height > visibleSize.height)){
			EditorPage->setAnchorPoint(Point(1, 1));
		}
		else if ((pos.x + pageSize.width<visibleSize.width) && (pos.y + pageSize.height>visibleSize.height)){
			EditorPage->setAnchorPoint(Point(0, 1));
		}
		else{
			EditorPage->setAnchorPoint(Point::ZERO);
		}

		EditorPage->setPosition(pos);
		//EditorPage->setAnchorPoint(Point(1, 1));
		this->addChild(EditorPage, 50);
		//deleteTowerPos(existPos);
	}

}
void TowerPosEditorLayer::recvDeletTowerBord(Ref* pData){
	TowerBorder* cur_towerbord = (TowerBorder*)pData;

	Point pos = cur_towerbord->getPosition();
	PosBase* existPos = findExistTowerPos(pos);
	log("Pos.x=%f,Pos.y=%f", pos.x, pos.y);
	clickBorder = NULL;
	if (existPos != NULL) {
	
		m_towerPosList.eraseObject(existPos);
	
		//m_monsterPosList.eraseObject(existPos);

		this->removeChild(existPos);
		
		Hero* delete_hero = cur_towerbord->getHero();
		this->removeChild(delete_hero);
		m_towerBorderList.eraseObject(cur_towerbord);
		this->removeChild(cur_towerbord);
		//deleteTowerPos(existPos);
	}

}

void TowerPosEditorLayer::recvRepairTowerBord(Ref* pData){
	TowerBorder* cur_towerbord = (TowerBorder*)pData;

	Point screenPos = cur_towerbord->getPosition();
	int totalBlood = cur_towerbord->getHero()->getiHP();
	auto repairLayer = Repair::create(screenPos,totalBlood);
	repairLayer->setPosition(Point::ZERO);
	repairLayer->setVisible(true);
	cur_towerbord->getHero()->addChild(repairLayer, 1000);

	this->repairVector.pushBack(repairLayer);


}

TowerBorder* TowerPosEditorLayer::findClickTowerBorder(Point pos){
	
	for (auto tBorder : m_towerBorderList){
		if (tBorder->isClickMe(pos)) {
			return tBorder;
		}

	}
	return NULL;
}

bool TowerPosEditorLayer::isWallAtTileCoord(const Point&tileCoord)const{
	return this->isPropAtTileCoordForLayer("Wall", tileCoord, bgLayer);
}
//�������Ҫ�ǲ����ˣ���ȥ�ģ������˷ֱ���Ӧ���ǲ��Եġ�
/*
Point TowerPosEditorLayer::tileCoordForPosition(const Point &position) const
{
	int x = (position.x + offX) / map->getContentSize().width*map->getMapSize().width;
	//	int x = (position.x) / map->getContentSize().width*map->getMapSize().width;

	int y = map->getMapSize().height - position.y / map->getContentSize().height*map->getMapSize().height;

	//û���÷ֱ��ʵ�
	//int x = position.x / map->getTileSize().width;
	//int y = ((map->getMapSize().height*map->getTileSize().height) - position.y) / map->getTileSize().height;
	return Point(x, y);
}
Point TowerPosEditorLayer::positionForTileCoord(const Point &tileCoord)const
{
	int x = tileCoord.x / map->getMapSize().width*map->getContentSize().width - offX;
	int y = (map->getMapSize().height - tileCoord.y) / map->getMapSize().height*map->getContentSize().height;

	//û���÷ֱ��ʵ�
	//int x = (tileCoord.x*map->getTileSize().width) + map->getTileSize().width / 2;
	//int y = (map->getMapSize().height*map->getTileSize().height) -
	//(tileCoord.y*map->getTileSize().height) - map->getTileSize().height / 2;
	return Point(x, y);
}*/

//����Ϊ���layer�϶���ʲô
//˵��ģ������Ǻ����ף���������֪���������Ƿ���һ��prop��������������ֵ��
bool TowerPosEditorLayer::isPropAtTileCoordForLayer(const char *prop, const Point &tileCoord, TMXLayer* layer)const
{
	if (!this->isValidTileCoord(tileCoord)){
		return false;
	}
	int gid = layer->getTileGIDAt(tileCoord);
	Value properties = map->getPropertiesForGID(gid);
	if (properties.isNull()){
		return false;
	}
	//prop��һ��char��Valuemap��һ��string����ô���ҵ�
	return properties.asValueMap().find(prop) != properties.asValueMap().end();
}
bool TowerPosEditorLayer::isValidTileCoord(const Point&tileCoord)const{
	if (tileCoord.x < 0 || tileCoord.y < 0 ||
		tileCoord.x >= map->getMapSize().width ||
		tileCoord.y >= map->getMapSize().height)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void TowerPosEditorLayer::addTower(Point pos){
	Hero* hero = Hero::createForSelect(heroType, bulletType);
	TowerPos* tPos = TowerPos::create(pos, 1, 1, true);
	this->addChild(tPos, 10);
	TowerBorder* create_border = TowerBorder::create();
	//�鷳�����������upgrade������Ϊ�˼�����sprite��
	create_border->upgrade();
	create_border->upgrade();
	create_border->setPosition(tPos->getPos());
	this->addChild(create_border);
	int heroType = tPos->getHeroType();
	int bulletType = tPos->getBulletType();

	//hero->setPosition(tPos->getPos());
	//this->addChild(hero, TOWER_LAYER_LVL);
	//create_border->bindHero(hero);

	//Hero* hero = Hero::createFromCsvFileByID(heroType, bulletType);
	hero->setPosition(tPos->getPos());
	this->addChild(hero, TOWER_LAYER_LVL);
	create_border->bindHero(hero);

	m_towerBorderList.pushBack(create_border);
	m_towerPosList.pushBack(tPos);
}