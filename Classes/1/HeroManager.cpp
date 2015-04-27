#include "HeroManager.h"
#include "PosLoadUtil.h"
#include "PosBase.h"
#include "TowerPos.h"

#include "Hero.h"
#include "Monster.h"
#include "GlobalDefine.h"


/*这里添加炮塔的逻辑是，当点击的时候，判断是否点击到炮台上，如果没有的话，则移除其他的东西，然后显示一个红叉，
如果点击到炮台上，则判断是否有英雄，如果有，则调用TowerBoard的显示3个升级按钮的东西，如果没有英雄，则调用
heropanle，此时heropanle点击是swallow掉其他点击的（在这个layer范围内），如果点击了该layer的一个图标后，则调用notify-post的消息
传递回给heromanager（这个消息没有弄，到时候可以改一下，主要是ref如何转化为其他形式还不太明白），然后调用hero create的方法，
添加到heromanager上，并将其绑定到towerborder上，以便于碰撞检测。
修改的地方（声明了一个recvTBPosition的变量，每次clickboard改变的时候，它就改变recvTBPosition = clickboard->getposition（）;
以便于创建hero和绑定炮台的时候，能确定是哪一个。（这里以后还要改一下，不太合适）

heromanager不在添加点击检测了。2月24日
*/

//这个东西根据tmx的宽高不同而不同，这里的tmx是9*16的。
#define MAP_WIDTH (16)//map的宽和高，可以调整
#define MAP_HEIGHT (9)

HeroManager::HeroManager() {

}
HeroManager::~HeroManager() {
	NOTIFY->removeObserver(this, "HeroType");

}

HeroManager* HeroManager::createWithLevel(int iCurLevel) {
    HeroManager* heroMgr = new HeroManager();
    if (heroMgr && heroMgr->initWithLevel(iCurLevel)) {
        heroMgr->autorelease();
    }
    else {
        CC_SAFE_DELETE(heroMgr);
    }
    return heroMgr;
}

/*新添加的，为了添加heromap，不能用，对非静态成员“heromanager：：map的非法引用。
HeroManager* HeroManager::createWithLevelAndMap(int iCurLevel,TMXTiledMap* MapLayermap) {
	HeroManager* heroMgr = new HeroManager();
	map = MapLayermap;
	if (heroMgr && heroMgr->initWithLevel(iCurLevel)) {
		heroMgr->autorelease();
	}
	else {
		CC_SAFE_DELETE(heroMgr);
	}
	return heroMgr;
}*/


bool HeroManager::initWithLevel(int iCurLevel) {
    /* 加载塔坐标对象 */
    std::string sTowerPosPath = StringUtils::format("tollgate/towerPos_level_%d.plist", iCurLevel);
	/*无所谓，map传不进来就再创建一次，
	std::string sBG = StringUtils::format("tollgate/level_%d.tmx", iCurLevel);
	map = TMXTiledMap::create(sBG.c_str());
	bgLayer = map->getLayer("bg");
	objects = map->getObjectGroup("obj");*/
	
    Vector<PosBase*> posList = PosLoadUtil::getInstance()->loadPosWithFile(
        sTowerPosPath.c_str(), enTowerPos, this, 10, false);//返回一个Vector<PosBase*>posList的容器包括所有的点的坐标
    m_towerPosList.pushBack(posList);//push all elements of an existing vector to the end of current vector.把所有的poslist里的elements都加到m_towerposlist里

	//choosepanle新添加
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Play.plist");//用frameswithfile的时候要预加载，研究一下，做一个loadscene的界面，把全局变量都先加载了。
	
	//clickBorder = NULL;
	//clickBorder = TowerBorder::create();
	//log("clickboard=%f,%f", clickBorder->getPosition().x, clickBorder->getPosition().y);
	//clickBorder = NULL;
	//this->addChild(clickBorder);
	//clickBorder = NULL;


    /* 创建炮台 ，其实是取炮台坐标，add炮台*/
    createTowerBorder(iCurLevel);
    /* 添加触摸监听，这个touch函数是否应该放在init里？到时候修改一下 
  //  auto listener = EventListenerTouchOneByOne::create();
	//touchListener->onTouchBegan = CC_CALLBACK_2(TollgateMapLayer::onTouchBegan, this);
	//touchListener->onTouchBegan = CC_CALLBACK_2(TollgateMapLayer::onTouchBegan, this);
 //  listener->onTouchBegan = [&](Touch* touch, Event* event){
	 //点击其他地方的时候，移除这两个东西，没有初始化，不知道会不会出错
	//   this->removeChild(chooseTowerpanel);
	//  chooseTowerpanel = NULL;
	  //想把它移除，这里好像不行。总是检测不存在NULL，就是说还没有创建click，直接调用的话，会出错。
	  //稍微优化一下，先在创建的时候，创建之前移除一下？
	 // 是因为clickboard变了，所以不能移除。以后再说吧。
	 // 添加一个检测clickboarder是否存在，并且把deleteoprbtns调整一下位置即可
	//  if (clickBorder!=NULL){
//		   clickBorder->deleteOprBtns();
//	  }
	
	 
//
//	   return true;
    };
    listener->onTouchEnded = [&](Touch* touch, Event* event){
        Point pos = Director::getInstance()->convertToGL(touch->getLocationInView());

        //获取被单击的塔坐标
        clickBorder = findClickTowerBorder(pos);//藏起来试试TowerBorder

//试一下，点击其他地方的时候把那三个东西归零。
        if (clickBorder == NULL) {
		//添加no.png
			auto tips = Sprite::createWithSpriteFrameName("no.png");
			tips->setPosition(pos);
			this->addChild(tips);
			tips->runAction(Sequence::create(DelayTime::create(0.8f),
				CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, tips)),
				NULL));
			
			if (chooseTowerpanel = NULL){
				return;
			}
			//this->removeChild(chooseTowerpanel);
			//这个到时候再移除。先加一个试试,应该在began哪里
			return;
        }
		//clickBorder->deleteOprBtns();

		
		//当前塔坐标没有英雄对象，则添加英雄 
		//把那个heropanleLayer加到这里，如果没有英雄，则添加layer
        if (clickBorder->getHero() == NULL) {
			auto location = clickBorder->getPosition();

			this->removeChild(chooseTowerpanel);
			chooseTowerpanel = NULL;
			//auto location = touch->getLocation();
			addTowerChoosePanel(location);
			recvTBPosition = location;
			log("clickboard=%f,%f", clickBorder->getPosition().x, clickBorder->getPosition().y);

			//addTower();
			//
			//Hero* hero = Hero::createFromCsvFileByID(1);
           // hero->setPosition(clickBorder->getPosition());
           // this->addChild(hero, TOWER_LAYER_LVL);
            //绑定英雄对象到炮台 
           // clickBorder->bindHero(hero);
        }
        else {
			clickBorder->showTowerOprBtns();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);*/

	/*添加消息，调用heroCreate。
	NOTIFY->addObserver(this,
		callfuncO_selector(HeroManager::recvTowerHeroType),
		"HeroType",
		NULL);*/

    return true;
}

//这个没用，搞错地方了。
/*void HeroManager::createTowerPos(Point pos) {
    TowerPos* tPos = TowerPos::create(pos, true);
    this->addChild(tPos, TOWER_POS_LAYER_LVL);
    m_towerPosList.pushBack(tPos);
}*/
void HeroManager::createTowerBorder(int iCurLevel) {
    for (auto tPos : m_towerPosList) {
        border = TowerBorder::create();

		//麻烦不，添加两次upgrade，就是为了加两个sprite？
		border->upgrade();
        border->upgrade();
        border->setPosition(tPos->getPos());
		//auto size = border->getContentSize();
		//auto physicsBody = PhysicsBody::createBox(size);
		//physicsBody->setDynamic(false);
		//border->setPhysicsBody(physicsBody);
		

		this->addChild(border);

		//这里不知道怎么回事，添加的时候sprite和物理总不在一起。
		//添加英雄及子弹
		int heroType = tPos->getHeroType();
		int bulletType = tPos->getBulletType();
		Hero* hero = Hero::createFromCsvFileByID(heroType,bulletType);
		auto size = hero->getSprite()->getContentSize();
	//	hero->setAnchorPoint(Point::ZERO);
	//	hero->setAnchorPoint(Point(0.5, 0.5));
		Point pos = tPos->getPos();
		hero->setPosition(pos);
	//	hero->setPosition(Point(pos.x+size.width/2,pos.y+size.height/2));
	
		auto physicsBody = PhysicsBody::createBox(size);
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(HERO_CATEGORY_BITMASK);
		physicsBody->setCollisionBitmask(HERO_COLLISION_BITMASK);
		physicsBody->setContactTestBitmask(HERO_CONTACTTEST_BITMASK);
		physicsBody->setTag(DRAG_BODYS_TAG);
		hero->setPhysicsBody(physicsBody);

		
		this->addChild(hero, TOWER_LAYER_LVL);
		border->bindHero(hero);

		m_towerBorderList.pushBack(border);
    }
}
/*
TowerBorder* HeroManager::findClickTowerBorder(Point pos){
    for (auto tBorder : m_towerBorderList){
        if (tBorder->isClickMe(pos)) {
            return tBorder;
        }

    }
    return NULL;
}
*/
//这个monsterlist应该是从maplayer里传进来的，m_heroMgr->logic(dt, m_monsterMgr->getMonsterList());

void HeroManager::logic(float dt, Vector<Monster*> monsterList) {
    
	for (auto tBorder : m_towerBorderList){
        if (tBorder->getHero() != NULL) {
            tBorder->getHero()->checkAtkMonster(dt, monsterList);
        }
    }
}


/*根据传入的位置，添加heropanellayer。
void HeroManager::addTowerChoosePanel(Point point){
	chooseTowerpanel = HeroPanleLayer::create();
	chooseTowerpanel->setPosition(point);
	this->addChild(chooseTowerpanel);
}

void HeroManager::recvTowerHeroType(Ref* pData){
	//chooseTowerpanel = (HeroPanleLayer)pData;
	if (clickBorder != NULL){
		
	int herotype = (int)pData;
	
	Hero* hero = Hero::createFromCsvFileByID(herotype,1);
		log("herotype=%d", herotype);

	hero->setPosition(clickBorder->getPosition());

	this->addChild(hero, TOWER_LAYER_LVL);
	clickBorder->bindHero(hero);


	auto type = HeroType::ANOTHER;
	chooseTowerpanel->setChooseHeroType(type);
	this->removeChild(chooseTowerpanel);
	chooseTowerpanel = NULL;
	}
}
*/
/*
//转换成tile的坐标，研究一下
Point HeroManager::covertTotileCoord(Point position){
	//如果设置了分辨率，即director->setContentScaleFactor(864.0f / 320.0f);则以下是正确的
	int x = (position.x + offX) / map->getContentSize().width*map->getMapSize().width;
	int y = map->getMapSize().height - position.y / map->getContentSize().height*map->getMapSize().height;

	//如果没有设置分辨率，则以下是正确的。
	int x = (position.x + offX) / map->getContentSize().width * map->getMapSize().width;
	int y = map->getMapSize().height -( position.y / map->getContentSize().height*map->getMapSize().height);
	

	return Point(x, y);
	return position;
}
//应该是panlelayer上的坐标
Point HeroManager::covertToMatrixCoord(Point position){
	int x = (position.x + offX) / map->getContentSize().width*map->getMapSize().width;
	int y = position.y / map->getContentSize().height*map->getMapSize().height;

	return Point(x, y);
	return position;

}*/


/*
void HeroManager::checkAndAddTowerPanle(Point position){
	Point towerCoord = covertTotileCoord(position);

	Point matrixCoord = covertToMatrixCoord(position);

	int gid = bgLayer->getTileGIDAt(towerCoord);//取它的gid和flags.
	//前面bgLayer声明了两次，(就是在init时，应该直接bgLayer=。。。结果写成auto bgLayer=。。。)
	//以至于bgLayer取的时候取全局那个取不到，导致点击的坐标超出范围报错。

	
	auto tmp_value = map->getPropertiesForGID(gid);//返回gid的属性字典（属性值吧）
	if (tmp_value.getType() != Value::Type::MAP){
		return;
	}
	auto tileTemp = map->getPropertiesForGID(gid).asValueMap();//与tmp_value一样，但是是valuemap类型的。
	int MatrixIndex = static_cast<int>(matrixCoord.y*MAP_WIDTH + matrixCoord.x);//？？

	int TouchValue;
	if (tileTemp.empty()){
		TouchValue = 0;
	}
	else
	{
		TouchValue = tileTemp.at("canTouch").asInt();
		log("TouchValue=%d", TouchValue);
	}

	//以下为坐标转化，就是检测点击的时候，是否是在panle上，如果是，则添加，如果不是，则显示no.png。
	//还要看一看
	auto tileWidth = map->getContentSize().width / map->getMapSize().width;
	auto tileHeight = map->getContentSize().height / map->getMapSize().height;

	//auto tileWidth = map->getContentSize().width / map->getMapSize().width;
	//auto tileHeight = map->getContentSize().height / map->getMapSize().height;
	//towerPos = Point((towerCoord.x * tileWidth) + tileWidth / 2 - offX, map->getContentSize().height - (towerCoord.y * tileHeight) - tileHeight / 2);


	towerPos = Point((towerCoord.x*tileWidth) + tileWidth / 2 - offX, map->getContentSize().height - (towerCoord.y*tileHeight) - tileHeight / 2);

	if (1 == TouchValue&&towerMatrix[MatrixIndex] == NULL)
	{
		addTowerChoosePanel(towerPos);
	}
	else
	{
		auto tips = Sprite::createWithSpriteFrameName("no.png");
		this->setPosition(towerPos);

		this->addChild(tips);
		tips->runAction(Sequence::create(DelayTime::create(0.8f),
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, tips)),
			NULL));
	}

}*/
/*
void HeroManager::addTower(){
	if (chooseTowerpanel){
		auto type = chooseTowerpanel->getChooseHeroType();
		if (type == HeroType::ANOTHER)
		{
			return;
		}

		
		Hero* hero = Hero::createFromCsvFileByID(1,1);
		 hero->setPosition(clickBorder->getPosition());
		 this->addChild(hero, TOWER_LAYER_LVL);
		//绑定英雄对象到炮台 
		 clickBorder->bindHero(hero);


		type = HeroType::ANOTHER;
		chooseTowerpanel->setChooseHeroType(type);
		this->removeChild(chooseTowerpanel);
		chooseTowerpanel = NULL;
	}
}
*/