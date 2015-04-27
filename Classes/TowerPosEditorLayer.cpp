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


#define HOME_LAYER_LVL 3  // 堡垒的层次
#define HERO_LAYER_LVL 10    // 英雄的层次
#define MONSTER_LAYER_LVL 15    // 怪物的层次
#define BULLET_LAYER_LVL 20  // 子弹的层次
#define TOWER_LAYER_LVL 10 
#define MAP_WIDTH (16)//map的宽和高，可以调整
#define MAP_HEIGHT (9)

TowerPosEditorLayer::TowerPosEditorLayer() {
   
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
	
	//m_heroMgr = HeroManager::createWithLevel(m_iCurLevel);
	//this->addChild(m_heroMgr, HERO_LAYER_LVL);
	/* 添加背景音乐 */
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
		StringUtils::format("music/tollgate_%d.mp3", m_iCurLevel).c_str(), true);
	
	std::string sTowerPosPath = StringUtils::format("tollgate/towerPos_level_%d.plist", m_iCurLevel);
	Vector<PosBase*> posList = PosLoadUtil::getInstance()->loadPosWithFile(
		sTowerPosPath.c_str(), enTowerPos, this, 10, false);//返回一个Vector<PosBase*>posList的容器包括所有的点的坐标
	m_towerPosList.pushBack(posList);//

	/**/
	for (auto tPos : m_towerPosList) {
		TowerBorder* border = TowerBorder::create();
		border->upgrade();
		border->upgrade();
		border->setPosition(tPos->getPos());
		this->addChild(border);
		//添加英雄及子弹
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
//编辑hero和bullet以及删除towerbord
	NOTIFY->addObserver(this,
		callfuncO_selector(TowerPosEditorLayer::recvRefreshHeroAndBulletType),
		"HeroAndBulletTypeChange",
		NULL);
	NOTIFY->addObserver(this,
		callfuncO_selector(TowerPosEditorLayer::recvDeletTowerBord),
		"DeletTowerBord",
		NULL);

    /* 监听触摸事件 */
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
        /* 判断当前编辑器的模式，进行不同的操作 */
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
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    /* 读取配置文件 */
    loadConfigFile();
    return true;
}
void TowerPosEditorLayer::initconfig(){
	std::string sTowerPosPath = StringUtils::format("tollgate/towerPos_level_%d.plist", m_iCurLevel);
	Vector<PosBase*> posList = PosLoadUtil::getInstance()->loadPosWithFile(
		sTowerPosPath.c_str(), enTowerPos, this, 10, false);
	m_towerPosList.pushBack(posList);
	outputPosToPlistFile();
}

/*编辑英雄和子弹
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
    /* 输出炮台坐标配置文件 */
    std::string sTowerPosPath = StringUtils::format("tollgate/towerPos_level_%d.plist", m_iCurLevel);
    outputPosToPlistFile_hero(m_towerPosList, sTowerPosPath.c_str());

    /* 输出怪物坐标配置文件,这个按道理说应该不用，但是暂时先这样，以后再改 */
    std::string sMonsterPosPath = StringUtils::format("tollgate/monsterPos_level_%d.plist", m_iCurLevel);
    outputPosToPlistFile(m_monsterPosList, sMonsterPosPath.c_str());
}

void TowerPosEditorLayer::outputPosToPlistFile_hero(Vector<PosBase*> posList, const char* sFilePath) {
    
	//ValueMap fileDataMap = FileUtils::getInstance()->getValueMapFromFile(sFilePath);
	ValueMap fileDataMap;
    /* 各个属性 */
   
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
	/* 各个属性 */

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

	offX = (map->getContentSize().width - winSize.width) / 2;//修正坐标，使点击的位置与tilemap的位置相同。

	bgLayer->setAnchorPoint(Point(0.5f, 0.5f));
	bgLayer->setPosition(Point(winSize.width / 2-offX, winSize.height / 2));

	

	this->addChild(map, -1);


    /* 加载塔坐标对象 
    std::string sTowerPosPath = StringUtils::format("tollgate/towerPos_level_%d.plist", m_iCurLevel);
    Vector<PosBase*> posList = PosLoadUtil::getInstance()->loadPosWithFile(sTowerPosPath.c_str(), enTowerPos, this, 10, true);
    m_towerPosList.pushBack(posList);*/

    /* 加载怪物坐标对象 */
    std::string sMonsterPosPath = StringUtils::format("tollgate/monsterPos_level_%d.plist", m_iCurLevel);
   Vector<PosBase*> posList = PosLoadUtil::getInstance()->loadPosWithFile(sMonsterPosPath.c_str(), enMonsterPos, this, 10, true);
    m_monsterPosList.pushBack(posList);
}

void TowerPosEditorLayer::editMonsterPos(Point pos) {
    /* 如果单击了已经存在的怪物坐标对象，则删除该坐标对象，否则创建新坐标对象 */
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
    deleteAllPos();		/* 删除当前所有的坐标对象 */
    m_iCurLevel++;		/* 关卡计数加1 */
    loadConfigFile();	/* 重新读取配置文件 */

    return m_iCurLevel;
}

int TowerPosEditorLayer::preLvl() {
    deleteAllPos();		/* 删除当前所有的坐标对象 */
    m_iCurLevel--;		/* 关卡计数减1 */
    loadConfigFile();	/* 重新读取配置文件 */

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
//貌似不需要上边两个也能成功，把this传出来即可
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
	//修改显示的塔和子弹
	
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


TowerBorder* TowerPosEditorLayer::findClickTowerBorder(Point pos){
	log("1");
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
//这个东西要是不对了，就去改，设置了分辨率应该是不对的。
Point TowerPosEditorLayer::tileCoordForPosition(const Point &position) const
{
	int x = (position.x + offX) / map->getContentSize().width*map->getMapSize().width;
	//	int x = (position.x) / map->getContentSize().width*map->getMapSize().width;

	int y = map->getMapSize().height - position.y / map->getContentSize().height*map->getMapSize().height;

	/*没设置分辨率的
	int x = position.x / map->getTileSize().width;
	int y = ((map->getMapSize().height*map->getTileSize().height) - position.y) / map->getTileSize().height;*/
	return Point(x, y);
}
Point TowerPosEditorLayer::positionForTileCoord(const Point &tileCoord)const
{
	int x = tileCoord.x / map->getMapSize().width*map->getContentSize().width - offX;
	int y = (map->getMapSize().height - tileCoord.y) / map->getMapSize().height*map->getContentSize().height;

	/*没设置分辨率的
	int x = (tileCoord.x*map->getTileSize().width) + map->getTileSize().width / 2;
	int y = (map->getMapSize().height*map->getTileSize().height) -
	(tileCoord.y*map->getTileSize().height) - map->getTileSize().height / 2;*/
	return Point(x, y);
}

//以下为检测layer上都有什么
//说真的，还不是很明白，反正就是知道回馈它是否是一个prop（传进来的属性值）
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
	//prop是一个char，Valuemap是一个string，怎么能找到
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
	//麻烦不，添加两次upgrade，就是为了加两个sprite？
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