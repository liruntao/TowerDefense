/*
这里是关卡主要界面创建的地方，把点击创建塔的界面也添加在这里

为了添加碰撞检测，在单例globalclient里添加几个容器，存储bullet和monster，添加到这里，但是在这个layer释放的时候，要把那个单例释放掉。
*/
#include "TollgateMapLayer.h"
#include "SimpleAudioEngine.h"
#include "HeroManager.h"
#include "MonsterManager.h"
#include "Monster.h"
#include "BulletBase.h"
#include "BulletManager.h"
#include "MonsterPos.h"
#include "GlobalDefine.h"
#include "extensions\cocos-ext.h"
#include "HeroPanleLayer.h"

USING_NS_CC_EXT;
using namespace CocosDenshion;

#define HOME_LAYER_LVL 3  // 堡垒的层次
#define HERO_LAYER_LVL 10    // 英雄的层次
#define MONSTER_LAYER_LVL 15    // 怪物的层次
#define BULLET_LAYER_LVL 20  // 子弹的层次

#define MAP_WIDTH (16)//map的宽和高，可以调整
#define MAP_HEIGHT (9)

TollgateMapLayer::TollgateMapLayer()
: _scene(nullptr)
{
   // m_iCurLevel = GlobalClient::getInstance()->getiCurTollgateLevel();
	
	
}

TollgateMapLayer::~TollgateMapLayer() {
	
	pointsVector.clear();
	GlobalClient::getInstance()->clear();
	NOTIFY->removeAllObservers(this);
	deadMonster_vector.clear();
	BodyResource.clear();

}


bool TollgateMapLayer::init() {
	if (!Layer::init()) {
		return false;
	}

	//_scene = (TollgateScene*)(this->getParent());

//	_scene = dynamic_cast<TollgateScene*>(this->getParent());

	instance = GlobalClient::getInstance();
	m_iCurLevel = GlobalClient::getInstance()->getiCurTollgateLevel();
	/* 读取关卡配置 */
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Size winSize = Director::getInstance()->getWinSize();


	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Play.plist");//用frameswithfile的时候要预加载，研究一下，做一个loadscene的界面，把全局变量都先加载了。
	loadConfig();
	chooseTowerpanel = NULL;
	/* 创建英雄管理器 */
	m_heroMgr = HeroManager::createWithLevel(m_iCurLevel);
	this->addChild(m_heroMgr, HERO_LAYER_LVL);

	/* 创建怪物管理器 */
	m_monsterMgr = MonsterManager::createWithLevel(m_iCurLevel, this);
	this->addChild(m_monsterMgr, MONSTER_LAYER_LVL);

	/* 创建起始点 */
	createStartPoint();

	/* 创建终点 */
	createEndPoint();

	
	NOTIFY->addObserver(this,
		callfuncO_selector(TollgateMapLayer::recvRefreshMonsterDeadBody),
		"MonsterDeadBody",
		NULL);

	Size size = CCDirector::getInstance()->getWinSize();
	auto s_visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();//Rect VisibleRect::getVisibleRect()
	auto visibleRectCenter = Point(s_visibleRect.origin.x + s_visibleRect.size.width / 2, s_visibleRect.origin.y + s_visibleRect.size.height / 2);//Vec2 VisibleRect::center()
	auto node = Node::create();
	node->setPhysicsBody(PhysicsBody::createEdgeBox(s_visibleRect.size));
	node->setPosition(visibleRectCenter);
	this->addChild(node);


	//碰撞检测
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(TollgateMapLayer::onContactBegin, this);
	contactListener->onContactSeperate = CC_CALLBACK_1(TollgateMapLayer::onContactSeperate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);


	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(TollgateMapLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(TollgateMapLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(TollgateMapLayer::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	
		scheduleUpdate();

	//	this->schedule(schedule_selector(TollgateMapLayer::SlowCheck), 0.1f);
		this->schedule(schedule_selector(TollgateMapLayer::logic));

	return true;
}


void TollgateMapLayer::loadConfig() {

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Size winSize = Director::getInstance()->getWinSize();

    /* 添加背景音乐 */
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
        StringUtils::format("music/tollgate_%d.mp3", m_iCurLevel).c_str(), true);

    /* 添加地图背景 
    std::string sBG = StringUtils::format("tollgate/level_%d.jpg", m_iCurLevel);
    Sprite* map = Sprite::create(sBG.c_str());
    map->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(map, 1);*/

	//这里就是取m_icurlevel的值把它添加到tollgate/level _中，然后使用,算是一个技巧
	std::string sBG = StringUtils::format("tollgate/level_%d.tmx",m_iCurLevel);
	map = TMXTiledMap::create(sBG.c_str());
	bgLayer = map->getLayer("bg");
	_meta = map->getLayer("Meta");
	_meta->setVisible(false);
	_propertiLayer = map->getLayer("properties");
	
	//新添加，这个offx是修正x的，因为是FIXED_HEIGHT，就是高度保持不变，因此，x值肯定会有偏差。Resources，design和screen有差别
	offX = (map->getContentSize().width - winSize.width) / 2;//修正坐标，使点击的位置与tilemap的位置相同。
//	objects = map->getObjectGroup("obj");
//	initBodyResourceVector();
	
	bgLayer->setAnchorPoint(Point(0.5f, 0.5f));
	//bgLayer->setPosition(Point(winSize.width / 2, winSize.height / 2));
	bgLayer->setPosition(Point(winSize.width / 2-offX, winSize.height / 2));

	

	this->addChild(map, -1);

	
	//initPointsVector(offX);

}

void TollgateMapLayer::logic(float dt) {

	m_heroMgr->logic(dt, m_monsterMgr->getMonsterList());
	//CollisionDetection();


}

void TollgateMapLayer::createEndPoint() {
    MonsterPos* pos = m_monsterMgr->getMonsterEndPos();

    Sprite* home = Sprite::create("sprite/end.png");
    home->setPosition(pos->getPos());

    auto rotateBy = RotateBy::create(15.0f, 360, 360);
    auto repeat = RepeatForever::create(rotateBy);
    home->runAction(repeat);


    this->addChild(home, HOME_LAYER_LVL);
}

void TollgateMapLayer::createStartPoint() {
    MonsterPos* pos = m_monsterMgr->getMonsterStartPos();

    Sprite* startSp = CCSprite::create("sprite/start.png");
    startSp->setPosition(pos->getPos());

    auto* rotateBy = RotateBy::create(15.0f, 360, 360);
    auto* repeat = RepeatForever::create(rotateBy);
    startSp->runAction(repeat);

    this->addChild(startSp, HOME_LAYER_LVL);
}

void TollgateMapLayer::initData(){
    /* 初始化塔魂、怪物和魔力数量 */
    int iTowerSoulNum = 50;                                          /* 塔魂数量 */
    int iMonsterNum = m_monsterMgr->getNotShowMonsterCount();       /* 怪物数量 */
    int iMagicNum = 1000;                                            /* 魔力数量 */

    NOTIFY->postNotification("TowerSoulChange", (Ref*)iTowerSoulNum);
    NOTIFY->postNotification("MonsterNumChange", (Ref*)iMonsterNum);
    NOTIFY->postNotification("MagicChange", (Ref*)iMagicNum);
}

bool TollgateMapLayer::onTouchBegan(Touch* touch, Event* event)
{
	if (_scene == NULL){
		_scene = (TollgateScene*)(this->getParent());

		auto m_world = _scene->getPhysicsWorld();
		log("1");
	}
	auto location = touch->getLocation();
	auto arr = _scene->getPhysicsWorld()->getShapes(location);
	
	PhysicsBody* body = nullptr;
	for (auto& obj : arr)
	{
		if (obj->getBody()->getNode()->getTag() == MONSTERBODYTAG){
			this->removeChild(obj->getBody()->getNode());
		}
	/*	if ((obj->getBody()->getTag() & DRAG_BODYS_TAG) != 0)
	{
	body = obj->getBody();
	break;
	}*/
	}
	/*
	if (body != nullptr)
	{
	Node* mouse = Node::create();
	mouse->setPhysicsBody(PhysicsBody::create(PHYSICS_INFINITY, PHYSICS_INFINITY));
	mouse->getPhysicsBody()->setDynamic(false);
	mouse->setPosition(location);
	this->addChild(mouse);
	PhysicsJointPin* joint = PhysicsJointPin::construct(mouse->getPhysicsBody(), body, location);
	joint->setMaxForce(5000.0f * body->getMass());
	_scene->getPhysicsWorld()->addJoint(joint);
	_mouses.insert(std::make_pair(touch->getID(), mouse));

	return true;
	}
*/
	//return false;
	return true;
}

void TollgateMapLayer::onTouchMoved(Touch* touch, Event* event)
{
	/*	auto it = _mouses.find(touch->getID());

	if (it != _mouses.end())
	{
	it->second->setPosition(touch->getLocation());
	}
	*/
}

void TollgateMapLayer::onTouchEnded(Touch* touch, Event* event)
{
	/*	auto it = _mouses.find(touch->getID());

	if (it != _mouses.end())
	{
	this->removeChild(it->second);
	_mouses.erase(it);
	}*/
	auto location = touch->getLocation();

//	addPhysics(location);
}

//deadMonster设置,就是将地图里名字为bodyresource的obj层里的数据读出来，放入BodyResource这个vector里
//当有monster死亡以后，也根据需求将其放入Bodyresource的vector里。
void TollgateMapLayer::recvRefreshMonsterDeadBody(Ref* pData){
	Monster* deadMonster = (Monster*)pData;
	deadMonster->convertToNodeSpace(this->getPosition());
	Point deadMonsterPosition = deadMonster->getPosition();

	
	Sprite* monsterSprite1 = Sprite::create("card_3.png");
	Point deadMonsterTilemapPos = this->tileCoordForPosition(deadMonsterPosition);
	auto size = monsterSprite1->getContentSize();

	Monster* monsterSprite = Monster::create();
	monsterSprite->bindSprite(monsterSprite1);
	monsterSprite->m_isDead = true;
	auto physicsBody = PhysicsBody::createBox(size);
	physicsBody->setDynamic(true);
	physicsBody->setCategoryBitmask(MONSTERBODY_CATEGORY_BITMASK);
	physicsBody->setCollisionBitmask(MONSTERBODY_COLLISION_BITMASK);
	physicsBody->setContactTestBitmask(MONSTERBODY_CONTACTTEST_BITMASK);
	physicsBody->setGravityEnable(false);
	//curBullet->getSprite()->setRotation(rotateDegrees);
	//deadMonster->setTag(MONSTERBODYTAG);
	//deadMonster->setPhysicsBody(physicsBody);
	monsterSprite->setTag(MONSTERBODYTAG);
	monsterSprite->setPhysicsBody(physicsBody);

//	deadMonster->setPosition(deadMonsterPosition);
//	this->addChild(deadMonster);

	monsterSprite->setPosition(deadMonsterPosition);
	this->addChild(monsterSprite);
	int deadMonsterValue = 3 * deadMonster->getiLevel();
	
	

}


//先将碰撞检测添加进来，
void TollgateMapLayer::CollisionDetection(){
	auto bulletVector = instance->BulletVector;
	auto monsterVector = instance->MonsterVector;



	if (bulletVector.empty() || monsterVector.empty()){
		//log("No bullet or monster!!");
		return;
	}

	//log("Have Two!");

	//以下碰撞检测见word文档碰撞检测-坐标转换
	Vector<Monster*> monsterNeedToDelete;
	Vector<BulletBase*>bulletNeedToDelete;
	for (int i = 0; i < bulletVector.size(); i++){
		auto bullet = bulletVector.at(i);
		if (bullet->getParent() == NULL)
		{
			//log("No bulletManager!");
			return;
		}
	

		Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());

		auto  bulletRect = Rect((bullet->convertToNodeSpace(heroWorldPos)).x - bullet->getSprite()->getContentSize().width / 2,
			(bullet->convertToNodeSpace(heroWorldPos)).y - bullet->getSprite()->getContentSize().height / 2,
			bullet->getContentSize().width,
			bullet->getContentSize().height);



		for (int j = 0; j < monsterVector.size(); j++){
			Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());

			auto monster = monsterVector.at(j);
			//这个monster的rect可能要改，因为添加的地方不一样，例子里是添加到maplayer上，我这里是添加到monsterlayer上
			auto monsterRect = Rect(monster->convertToNodeSpace(heroWorldPos).x - monster->getContentSize().width / 4,
				monster->convertToNodeSpace(heroWorldPos).y - monster->getContentSize().height / 4,
				monster->getContentSize().width / 2,
				monster->getContentSize().height / 2);


			Vector<Monster*> monsterNeedToHurt;
			
			

			if (bulletRect.intersectsRect(monsterRect)){
				//扣怪物的血，还有子弹和怪物移除还没有写。
				//应该调用bulletmanager里的函数了，
				//调用bulletmanager里的logic函数（就是一直检测是否到达目的地）里的aim->hurtme（entity里的函数，这个那个都可以用啊。）
				
				//monsterNeedToHurt.pushBack(monster);
				if (monster != NULL){
				monster->hurtMe(bullet->getiAtkValue());}
				//bullet->getSprite()->removeFromParent();
				//不移除反复碰撞，但那样的话，就会导致反复碰撞？
				//instance->BulletVector.eraseObject(bullet);
				
				//这个后边再说，订阅的消息不知道会不会出问题。
				//int type = 1;
				//NOTIFY->postNotification("BulletType", (Ref*)bullet);
				//以后试试在这里，一旦碰撞了以后，就直接把bullet移除，而不用等到arrive以后再移除。
				//bullet->setUsed(false);
				log("bulletRect.x = %f, bulletRect.y = %f, bulletRect.w = %f, bulletRect.h = %f", (bullet->convertToNodeSpace(heroWorldPos)).x - bullet->getSprite()->getContentSize().width / 2,
					(bullet->convertToNodeSpace(heroWorldPos)).y - bullet->getSprite()->getContentSize().height / 2,
					bullet->getContentSize().width,
					bullet->getContentSize().height);
				log("monsterRect.x=%f,monsterRect.y=%f,monsterRect.w=%f,monsterRect.h=%f", monster->convertToNodeSpace(heroWorldPos).x - monster->getContentSize().width / 4,
					monster->convertToNodeSpace(heroWorldPos).y - monster->getContentSize().height / 4,
					monster->getContentSize().width / 2,
					monster->getContentSize().height / 2);
				log("11");

				break;
			}
			/*for (Monster* monsterTemp : monsterNeedToHurt){
				auto monsterNeedToHurtRect = Rect(monsterTemp->convertToNodeSpace(heroWorldPos).x - monsterTemp->getContentSize().width / 4,
					monsterTemp->convertToNodeSpace(heroWorldPos).y - monsterTemp->getContentSize().height / 4,
					monsterTemp->getContentSize().width / 2,
					monsterTemp->getContentSize().height / 2);

				if (bulletRect.intersectsRect(monsterNeedToHurtRect)==false){
					monsterTemp->hurtMe(bullet->getiAtkValue());
					monsterNeedToHurt.eraseObject(monsterTemp, false);
				}
			}*/
		}
	}

}

//以下为a*算法的一些函数
bool TollgateMapLayer::isWallAtTileCoord(const Point&tileCoord)const{
	return this->isPropAtTileCoordForLayer("Wall", tileCoord, bgLayer);
}
//这个东西要是不对了，就去改，设置了分辨率应该是不对的。
Point TollgateMapLayer::tileCoordForPosition(const Point &position) const
{
	int x = (position.x + offX) / map->getContentSize().width*map->getMapSize().width;
//	int x = (position.x) / map->getContentSize().width*map->getMapSize().width;

	int y = map->getMapSize().height - position.y / map->getContentSize().height*map->getMapSize().height;

	/*没设置分辨率的
	int x = position.x / map->getTileSize().width;
	int y = ((map->getMapSize().height*map->getTileSize().height) - position.y) / map->getTileSize().height;*/
	return Point(x, y);
}
Point TollgateMapLayer::positionForTileCoord(const Point &tileCoord)const
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
bool TollgateMapLayer::isPropAtTileCoordForLayer(const char *prop, const Point &tileCoord, TMXLayer* layer)const
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

//设置wall属性,set的要是vector
void TollgateMapLayer::setPropAtTileCoordForLayer(const char *prop, const Point &tileCoord, TMXLayer* layer)const
{
	
	//for (auto tileCoord : tileCoord_vector)
	//{

		if (!this->isValidTileCoord(tileCoord)){
			return ;
		}
		int gid = layer->getTileGIDAt(tileCoord);
		Value properties = map->getPropertiesForGID(gid);
		ValueMap changePorp = properties.asValueMap();

		if (changePorp.size() == 0){
			ValueMap createMap;
			createMap[StringUtils::toString(prop)] = Value(1);
			layer->setProperties(createMap);
		}
		else
		{
		int type_int;
		type_int = changePorp[StringUtils::toString(prop)].asInt();
		
		if  (type_int < 9){
			changePorp[StringUtils::toString(prop)] = Value(type_int + 1);
		}
		//}
	
	


		/*Value properties = map->getPropertiesForGID(gid);
		if (properties.isNull()){
			return false;
		}
	
	return properties.asValueMap().find(prop) != properties.asValueMap().end();*/
	}
}
bool TollgateMapLayer::isValidTileCoord(const Point&tileCoord)const{
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
PointArray *TollgateMapLayer::walkableAdjacentTilesCoordForTileCoord(const Point &tileCoord) const
{
	PointArray *tmp = PointArray::create(8);

	bool t = false;
	bool l = false;
	bool b = false;
	bool r = false;

	// 上
	Point p(tileCoord.x, tileCoord.y - 1);
	if (this->isValidTileCoord(p) && !this->isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
		t = true;
	}

	// 左
	p.setPoint(tileCoord.x - 1, tileCoord.y);
	if (this->isValidTileCoord(p) && !this->isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
		l = true;
	}

	// 下
	p.setPoint(tileCoord.x, tileCoord.y + 1);
	if (this->isValidTileCoord(p) && !this->isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
		b = true;
	}

	// 右
	p.setPoint(tileCoord.x + 1, tileCoord.y);
	if (this->isValidTileCoord(p) && !this->isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
		r = true;
	}

	// 左上
	p.setPoint(tileCoord.x - 1, tileCoord.y - 1);
	if (t && l && this->isValidTileCoord(p) && !this->isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
	}

	// 左下
	p.setPoint(tileCoord.x - 1, tileCoord.y + 1);
	if (b && l && this->isValidTileCoord(p) && !this->isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
	}

	// 右上
	p.setPoint(tileCoord.x + 1, tileCoord.y - 1);
	if (t && r && this->isValidTileCoord(p) && !this->isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
	}

	// 右下
	p.setPoint(tileCoord.x + 1, tileCoord.y + 1);
	if (b && r && this->isValidTileCoord(p) && !this->isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
	}

	return tmp;
}



void TollgateMapLayer::addTowerChoosePanel(Point point){
	chooseTowerpanel = HeroPanleLayer::create();
	chooseTowerpanel->setPosition(point);
	this->addChild(chooseTowerpanel);
}
//转换成tile的坐标，研究一下
Point TollgateMapLayer::covertTotileCoord(Point position){
/*如果设置了分辨率，即director->setContentScaleFactor(864.0f / 320.0f);则以下是正确的*/
	int x = (position.x+offX)  / map->getContentSize().width*map->getMapSize().width;
	int y = map->getMapSize().height - position.y / map->getContentSize().height*map->getMapSize().height;

	/*如果没有设置分辨率，则以下是正确的。
	int x = (position.x + offX) / map->getContentSize().width * map->getMapSize().width;
	int y = map->getMapSize().height -( position.y / map->getContentSize().height*map->getMapSize().height);
	*/

	return Point(x, y);
}



//
void TollgateMapLayer::checkAndAddTowerPanle(Point position){
	
	
}


//monster的value值,失败了
void TollgateMapLayer::ChangeMonsterTmxValue(){
	auto monsterVector = GlobalClient::getInstance()->MonsterVector;
	if (monsterVector.empty()){
		return;
	}
	for (auto a : monsterVector){
		
	//	Point pos = Point(a->convertToNodeSpace(this->getPosition()).x, a->convertToNodeSpace(this->getPosition()).y);
		Point pos = a->getPosition();
		//log("pos.x=%f,pos.y=%f", pos.x, pos.y);
		if (pos.x <= 0 || pos.y <= 0){ return; }
		Point tileCoord = this->tileCoordForPosition(pos);
		//log("tileCoord.x=%f,tileCoord.y=%f", tileCoord.x, tileCoord.y);
		int tileGid = _meta->getTileGIDAt(tileCoord);
		//log("gid=%d", tileGid);
		if (tileGid) {
			auto properties = map->getPropertiesForGID(tileGid).asValueMap();
			if (!properties.empty()) {
				int collectableNum = properties["Collectable"].asInt();
				if (collectableNum > 0) {
				int changeSpeed = -collectableNum * 10;
				log("changeSpeed=%d", changeSpeed);
				a->setchangeSpeed(changeSpeed);
				a->hurtMe(70);
				}	
			}
			
		}
		else
				{
					a->setchangeSpeed(0);
		}
		//属性掉血
		int propertiesGid = _propertiLayer->getTileGIDAt(tileCoord);
		if (propertiesGid){
			auto properties = map->getPropertiesForGID(propertiesGid).asValueMap();
			if (!properties.empty()){
				//if (!(properties.find("metalDamage") != properties.end())){		
					int damage = properties["metalDamage"].asInt();
					float monsterMetalDefense = a->getMetalDefense();
					float monsterWaterAtk = a->getWaterAtk();
					auto x = -(monsterMetalDefense / 100)*damage;
					int curDamage = (monsterMetalDefense / 100)*damage;
					int curHeal = (monsterWaterAtk)*damage;
					a->hurtMe(curDamage);
					a->hurtMe(curHeal);

				
		//	}
		}

}

};
	/*	//Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());

		//这个monster的rect可能要改，因为添加的地方不一样，例子里是添加到maplayer上，我这里是添加到monsterlayer上
	//	auto monsterPoint = Point(a->convertToNodeSpace(heroWorldPos).x, a->convertToNodeSpace(heroWorldPos).y);
	
	//	auto pos3 = Director::getInstance()->convertToGL(pos);
	//	auto pos1 = a->convertToNodeSpace(pos3);
	//	auto pos2 = this->convertToNodeSpace(pos);
		Point pos1 = this->convertToNodeSpace(pos);
		Point tileCoord = this->tileCoordForPosition(pos1);
		log("11");
	///	Point tileCoord1 = this->tileCoordForPosition(pos2);
	//	Point tileCoord2 = this->tileCoordForPosition(pos3);
	

//		Point tileCoord = this->tileCoordForPosition(pos1);

	//	int tileGid = _meta->getTileGIDAt(tileCoord);
		
		int tileGid = _meta->getTileGIDAt(tileCoord);
		if (tileGid) {
			auto properties = map->getPropertiesForGID(tileGid).asValueMap();
			if (!properties.empty()) {
				auto collectableNum = properties["Collectable"].asInt();
				if (collectableNum > 0) {
					int changeSpeed = -collectableNum*10 ;
					log("changeSpeed=%d", changeSpeed);
					a->setchangeSpeed(changeSpeed);

				}
			}
		}*/
	
	
}
//这个可以每1秒检测一次？
void TollgateMapLayer::SlowCheck(float dt){
	ChangeMonsterTmxValue();

	auto monsterVector = instance->MonsterVector;

	if (deadMonster_vector.empty() || monsterVector.empty()){
		//log("No bullet or monster!!");
		return;
	}


	//以下碰撞检测见word文档碰撞检测-坐标转换
	for (auto monster : monsterVector){
		Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());
	//	auto monsterRect = Rect(monster->convertToNodeSpace(heroWorldPos).x - monster->getContentSize().width / 4,
	//		monster->convertToNodeSpace(heroWorldPos).y - monster->getContentSize().height / 4,
	//		monster->getContentSize().width / 2,
	//		monster->getContentSize().height / 2);
		auto monsterRect = Rect(monster->convertToNodeSpace(heroWorldPos).x ,
			monster->convertToNodeSpace(heroWorldPos).y ,
			monster->getContentSize().width ,
			monster->getContentSize().height);


		int bodySurround = 0;

		for (auto checkDeadMonster : deadMonster_vector){
			Size size = checkDeadMonster->getContentSize();
		//	checkDeadMonster->convertToNodeSpace(heroWorldPos);
		//	Point cur_position = checkDeadMonster->getPosition();
		//	Point srcPos = Point(cur_position.x - size.width / 2, cur_position.y + size.height / 2);
		//	Point destPos = Point(cur_position.x + size.width / 2, cur_position.y - size.height / 2);
		//	Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());
			auto  checkDeadMonsterRect = Rect(checkDeadMonster->convertToNodeSpace(heroWorldPos).x,
				checkDeadMonster->convertToNodeSpace(heroWorldPos).y, size.width, size.height);

				if (checkDeadMonsterRect.intersectsRect(monsterRect)){
					++bodySurround;
				//	break;
				}
		    }
		monster->setchangebodySpeed(bodySurround);
		log("bodySurround=%d",bodySurround);
	}
}


//这是原来设计的，取地图的值，然后遍历,两个所耗资源差不错，但是上边那个扩展性要好。可以把所有地图种类都包含进去。
	//这里vlauemap不能删除特定元素，那就使用deadmonster的vector来检测吧。
	/*
auto monsterVector = instance->MonsterVector;

	if (BodyResource.empty() || monsterVector.empty()){
		//log("No bullet or monster!!");
		return;
	}


	//以下碰撞检测见word文档碰撞检测-坐标转换

	for (int i = 0; i < BodyResource.size(); i++){
		ValueMap bodyResouce = BodyResource.at(i).asValueMap();

		float x = bodyResouce.at("x").asFloat();
		float y = bodyResouce.at("y").asFloat() - 20;
		int type = bodyResouce.at("type").asInt();
		float width = bodyResouce.at("width").asFloat();
		float height = bodyResouce.at("height").asFloat();
		std::string ID = StringUtils::toString(x) + StringUtils::toString(y);
		//auto  bodyResouceRect = Rect(x, y, width, height);


		//试一下
		Node* runOfPoint = NULL;
		runOfPoint = Node::create();
		runOfPoint->setPosition(Point(x, y));
		Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());
		auto  bodyResouceRect = Rect(runOfPoint->convertToNodeSpace(heroWorldPos).x,
			runOfPoint->convertToNodeSpace(heroWorldPos).y, width, height);

		for (int j = 0; j < monsterVector.size(); j++){
			Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());

			auto monster = monsterVector.at(j);
			//这个monster的rect可能要改，因为添加的地方不一样，例子里是添加到maplayer上，我这里是添加到monsterlayer上
			auto monsterRect = Rect(monster->convertToNodeSpace(heroWorldPos).x - monster->getContentSize().width / 4,
				monster->convertToNodeSpace(heroWorldPos).y - monster->getContentSize().height / 4,
				monster->getContentSize().width / 2,
				monster->getContentSize().height / 2);
		
			if (bodyResouceRect.intersectsRect(monsterRect)){
				//减速,如何设计的更合理点
		log("bodyResouce.x=%f,bodyResouce.y=%f,bodyResouce.w=%f,bodyResouce.h=%f", runOfPoint->convertToNodeSpace(heroWorldPos).x,
					runOfPoint->convertToNodeSpace(heroWorldPos).y, width, height);
				log("monsterRect.x=%f,monsterRect.y=%f,monsterRect.w=%f,monsterRect.h=%f", monster->convertToNodeSpace(heroWorldPos).x - monster->getContentSize().width / 4,
					monster->convertToNodeSpace(heroWorldPos).y - monster->getContentSize().height / 4,
					monster->getContentSize().width / 2,
					monster->getContentSize().height / 2);
				auto original_Speed = monster->getMoveController()->getiSpeed();
				log("original_speed=%d", original_Speed);
				auto cur_Speed =  - type * 10;
				//monster->setiSpeed(cur_Speed);
				monster->setchangeSpeed(cur_Speed);
				
				break;
			}
			else
			{
				monster->setchangeSpeed(0);
			}
		}
	}*/
/*
void TollgateMapLayer::draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated){
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(TollgateMapLayer::onDraw, this, transform, transformUpdated);
	renderer->addCommand(&_customCommand);
}

void TollgateMapLayer::onDraw(const kmMat4 &transform, bool transformUpdated){
	kmGLPushMatrix();
	kmGLLoadMatrix(&transform);

	Vector<PosBase*> m_monsterPosList = m_editorLayer->getMonsterPosList();
	PosBase* prePos = NULL;
	for (auto posBase : m_monsterPosList) {
		if (prePos != NULL) {
			DrawPrimitives::drawLine(prePos->getPos(), posBase->getPos());
		}
		prePos = posBase;
	}

	kmGLPopMatrix();// 结束绘制
}*/
//如果不在map中返回的是一个map，wrong为true，否则，即使没有properties，也可以返回一个空properties。
ValueMap& TollgateMapLayer::getpropertiesValueMapAtTileCoord(const Point &tileCoord, TMXLayer *layer){
	ValueMap a;
	a["wrong"] = Value(1);
	if (!this->isValidTileCoord(tileCoord)){
		return a;
	}

	int gid = layer->getTileGIDAt(tileCoord);
	Value& b = map->getPropertiesForGID(gid);
   ValueMap& properties = b.asValueMap();
	
	//ValueMap& properties = map->getPropertiesForGID(gid).asValueMap();
	//if (properties.empty()){
	//	ValueMap b;
//	}
	return properties;

/*	if (!this->isValidTileCoord(tileCoord)){
		return false;
	}

	int gid = layer->getTileGIDAt(tileCoord);
	Value properties = map->getPropertiesForGID(gid);
	if (properties.isNull()){
		return false;
	}
	//prop是一个char，Valuemap是一个string，怎么能找到
	return properties.asValueMap().find(prop) != properties.asValueMap().end();*/
}

void TollgateMapLayer::addPhysics(Point position){
	auto physicsBody = PhysicsBody::createBox(Size(65.0f, 81.0f),
		PhysicsMaterial(0.1f, 1.0f, 1.0f));
	physicsBody->setDynamic(true);
	physicsBody->setCategoryBitmask(MONSTERBODY_CATEGORY_BITMASK);
	physicsBody->setCollisionBitmask(MONSTERBODY_COLLISION_BITMASK);
	physicsBody->setContactTestBitmask(MONSTERBODY_CONTACTTEST_BITMASK);
	physicsBody->setGravityEnable(false);
	physicsBody->setTag(MONSTERBODYTAG);
	//curBullet->getSprite()->setRotation(rotateDegrees);
	auto sprite = Sprite::create("btn_back.png");
	sprite->setPhysicsBody(physicsBody);
	sprite->setTag(MONSTERBODYTAG);
	sprite->setPosition(position);
	//set the body isn't affected by the physics world's gravitational force
	physicsBody->setGravityEnable(false);
	/*
	physicsBody->setDynamic(true);

	physicsBody->setCategoryBitmask(0x08);    // 1000
	physicsBody->setContactTestBitmask(0x08); // 0010
	physicsBody->setCollisionBitmask(0x01);   // 0001
	//set initial velocity of physicsBody
	//	physicsBody->setVelocity(Point(CCRANDOM_MINUS1_1()*500,
	//		CCRANDOM_MINUS1_1() * 500));
	//physicsBody->setTag(DRAG_BODYS_TAG);
	//physicsBody->setContactTestBitmask(0xFFFFFFFF);
	
	//physicsBody->setCollisionBitmask(0x01);

	auto sprite = Sprite::create("btn_back.png");
	sprite->setPosition(position);
	sprite->setPhysicsBody(physicsBody);
	//sprite->setTag(BULLET_ONETAG);*/
	this->addChild(sprite);
}

bool TollgateMapLayer::onContactBegin(PhysicsContact& contact){
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB){

		auto tagA = nodeA->getTag();
		auto tagB = nodeB->getTag();
		if (nodeA->getTag() == BULLET_ONETAG&&nodeB->getTag() == BULLET_ONETAG){
			return false;
		}
		else if (nodeA->getTag() == BULLET_ONETAG){
			auto bullet = (BulletBase*)nodeA;
			auto monster = (Entity*)nodeB;
			auto atkValue = bullet->getiAtkValue();
			monster->hurtMe(atkValue);
			nodeA->removeFromParentAndCleanup(true);
		}
		else if (nodeB->getTag() == BULLET_ONETAG){
			auto bullet = (BulletBase*)nodeB;
			auto monster = (Entity*)nodeA;
			auto atkValue = bullet->getiAtkValue();
			monster->hurtMe(20*atkValue);
			nodeB->removeFromParentAndCleanup(true);
		}
		else if (nodeA->getTag() == BULLET_SEVERALTAG&&nodeB->getTag() == BULLET_SEVERALTAG){
			return false;
		}
		else if (nodeA->getTag() == BULLET_SEVERALTAG){
			auto bullet = (BulletBase*)nodeA;
			auto monster = (Entity*)nodeB;
			auto atkValue = bullet->getiAtkValue();
			monster->hurtMe(atkValue);
		}
		else if (nodeB->getTag() == BULLET_SEVERALTAG){
			auto bullet = (BulletBase*)nodeB;
			auto monster = (Entity*)nodeA;
			auto atkValue = bullet->getiAtkValue();
			monster->hurtMe(atkValue);
		}
		else if (nodeA->getTag() == MONSTERTAG&&nodeB->getTag() == MONSTERBODYTAG){
			auto monster = (Entity*)nodeA;
			monster->setchangebodySpeed(30);
		}
		else if (nodeB->getTag() == MONSTERTAG&&nodeA->getTag() == MONSTERBODYTAG){
			auto monster = (Entity*)nodeB;
			monster->setchangebodySpeed(30);
		}
		
	}
	return true;
}

void TollgateMapLayer::onContactSeperate(PhysicsContact& contact){
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB){

		auto tagA = nodeA->getTag();
		auto tagB = nodeB->getTag();
		if (tagA == MONSTERBODYTAG&&tagB == MONSTERTAG){
			auto monster = (Entity*)nodeB;
			monster->setchangebodySpeed(0);
		}
		else if (tagA == MONSTERTAG&&tagB == MONSTERBODYTAG){
			auto monster = (Entity*)nodeA;
			monster->setchangebodySpeed(0);
		}
	

	}
	
}