#include "HeroManager.h"
#include "PosLoadUtil.h"
#include "PosBase.h"
#include "TowerPos.h"

#include "Hero.h"
#include "Monster.h"
#include "GlobalDefine.h"


/*��������������߼��ǣ��������ʱ���ж��Ƿ�������̨�ϣ����û�еĻ������Ƴ������Ķ�����Ȼ����ʾһ����棬
����������̨�ϣ����ж��Ƿ���Ӣ�ۣ�����У������TowerBoard����ʾ3��������ť�Ķ��������û��Ӣ�ۣ������
heropanle����ʱheropanle�����swallow����������ģ������layer��Χ�ڣ����������˸�layer��һ��ͼ��������notify-post����Ϣ
���ݻظ�heromanager�������Ϣû��Ū����ʱ����Ը�һ�£���Ҫ��ref���ת��Ϊ������ʽ����̫���ף���Ȼ�����hero create�ķ�����
��ӵ�heromanager�ϣ�������󶨵�towerborder�ϣ��Ա�����ײ��⡣
�޸ĵĵط���������һ��recvTBPosition�ı�����ÿ��clickboard�ı��ʱ�����͸ı�recvTBPosition = clickboard->getposition����;
�Ա��ڴ���hero�Ͱ���̨��ʱ����ȷ������һ�����������Ժ�Ҫ��һ�£���̫���ʣ�

heromanager������ӵ������ˡ�2��24��
*/

//�����������tmx�Ŀ�߲�ͬ����ͬ�������tmx��9*16�ġ�
#define MAP_WIDTH (16)//map�Ŀ�͸ߣ����Ե���
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

/*����ӵģ�Ϊ�����heromap�������ã��ԷǾ�̬��Ա��heromanager����map�ķǷ����á�
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
    /* ������������� */
    std::string sTowerPosPath = StringUtils::format("tollgate/towerPos_level_%d.plist", iCurLevel);
	/*����ν��map�����������ٴ���һ�Σ�
	std::string sBG = StringUtils::format("tollgate/level_%d.tmx", iCurLevel);
	map = TMXTiledMap::create(sBG.c_str());
	bgLayer = map->getLayer("bg");
	objects = map->getObjectGroup("obj");*/
	
    Vector<PosBase*> posList = PosLoadUtil::getInstance()->loadPosWithFile(
        sTowerPosPath.c_str(), enTowerPos, this, 10, false);//����һ��Vector<PosBase*>posList�������������еĵ������
    m_towerPosList.pushBack(posList);//push all elements of an existing vector to the end of current vector.�����е�poslist���elements���ӵ�m_towerposlist��

	//choosepanle�����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Play.plist");//��frameswithfile��ʱ��ҪԤ���أ��о�һ�£���һ��loadscene�Ľ��棬��ȫ�ֱ������ȼ����ˡ�
	
	//clickBorder = NULL;
	//clickBorder = TowerBorder::create();
	//log("clickboard=%f,%f", clickBorder->getPosition().x, clickBorder->getPosition().y);
	//clickBorder = NULL;
	//this->addChild(clickBorder);
	//clickBorder = NULL;


    /* ������̨ ����ʵ��ȡ��̨���꣬add��̨*/
    createTowerBorder(iCurLevel);
    /* ��Ӵ������������touch�����Ƿ�Ӧ�÷���init���ʱ���޸�һ�� 
  //  auto listener = EventListenerTouchOneByOne::create();
	//touchListener->onTouchBegan = CC_CALLBACK_2(TollgateMapLayer::onTouchBegan, this);
	//touchListener->onTouchBegan = CC_CALLBACK_2(TollgateMapLayer::onTouchBegan, this);
 //  listener->onTouchBegan = [&](Touch* touch, Event* event){
	 //��������ط���ʱ���Ƴ�������������û�г�ʼ������֪���᲻�����
	//   this->removeChild(chooseTowerpanel);
	//  chooseTowerpanel = NULL;
	  //������Ƴ�����������С����Ǽ�ⲻ����NULL������˵��û�д���click��ֱ�ӵ��õĻ��������
	  //��΢�Ż�һ�£����ڴ�����ʱ�򣬴���֮ǰ�Ƴ�һ�£�
	 // ����Ϊclickboard���ˣ����Բ����Ƴ����Ժ���˵�ɡ�
	 // ���һ�����clickboarder�Ƿ���ڣ����Ұ�deleteoprbtns����һ��λ�ü���
	//  if (clickBorder!=NULL){
//		   clickBorder->deleteOprBtns();
//	  }
	
	 
//
//	   return true;
    };
    listener->onTouchEnded = [&](Touch* touch, Event* event){
        Point pos = Director::getInstance()->convertToGL(touch->getLocationInView());

        //��ȡ��������������
        clickBorder = findClickTowerBorder(pos);//����������TowerBorder

//��һ�£���������ط���ʱ����������������㡣
        if (clickBorder == NULL) {
		//���no.png
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
			//�����ʱ�����Ƴ����ȼ�һ������,Ӧ����began����
			return;
        }
		//clickBorder->deleteOprBtns();

		
		//��ǰ������û��Ӣ�۶��������Ӣ�� 
		//���Ǹ�heropanleLayer�ӵ�������û��Ӣ�ۣ������layer
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
            //��Ӣ�۶�����̨ 
           // clickBorder->bindHero(hero);
        }
        else {
			clickBorder->showTowerOprBtns();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);*/

	/*�����Ϣ������heroCreate��
	NOTIFY->addObserver(this,
		callfuncO_selector(HeroManager::recvTowerHeroType),
		"HeroType",
		NULL);*/

    return true;
}

//���û�ã����ط��ˡ�
/*void HeroManager::createTowerPos(Point pos) {
    TowerPos* tPos = TowerPos::create(pos, true);
    this->addChild(tPos, TOWER_POS_LAYER_LVL);
    m_towerPosList.pushBack(tPos);
}*/
void HeroManager::createTowerBorder(int iCurLevel) {
    for (auto tPos : m_towerPosList) {
        border = TowerBorder::create();

		//�鷳�����������upgrade������Ϊ�˼�����sprite��
		border->upgrade();
        border->upgrade();
        border->setPosition(tPos->getPos());
		//auto size = border->getContentSize();
		//auto physicsBody = PhysicsBody::createBox(size);
		//physicsBody->setDynamic(false);
		//border->setPhysicsBody(physicsBody);
		

		this->addChild(border);

		//���ﲻ֪����ô���£���ӵ�ʱ��sprite�������ܲ���һ��
		//���Ӣ�ۼ��ӵ�
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
//���monsterlistӦ���Ǵ�maplayer�ﴫ�����ģ�m_heroMgr->logic(dt, m_monsterMgr->getMonsterList());

void HeroManager::logic(float dt, Vector<Monster*> monsterList) {
    
	for (auto tBorder : m_towerBorderList){
        if (tBorder->getHero() != NULL) {
            tBorder->getHero()->checkAtkMonster(dt, monsterList);
        }
    }
}


/*���ݴ����λ�ã����heropanellayer��
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
//ת����tile�����꣬�о�һ��
Point HeroManager::covertTotileCoord(Point position){
	//��������˷ֱ��ʣ���director->setContentScaleFactor(864.0f / 320.0f);����������ȷ��
	int x = (position.x + offX) / map->getContentSize().width*map->getMapSize().width;
	int y = map->getMapSize().height - position.y / map->getContentSize().height*map->getMapSize().height;

	//���û�����÷ֱ��ʣ�����������ȷ�ġ�
	int x = (position.x + offX) / map->getContentSize().width * map->getMapSize().width;
	int y = map->getMapSize().height -( position.y / map->getContentSize().height*map->getMapSize().height);
	

	return Point(x, y);
	return position;
}
//Ӧ����panlelayer�ϵ�����
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

	int gid = bgLayer->getTileGIDAt(towerCoord);//ȡ����gid��flags.
	//ǰ��bgLayer���������Σ�(������initʱ��Ӧ��ֱ��bgLayer=���������д��auto bgLayer=������)
	//������bgLayerȡ��ʱ��ȡȫ���Ǹ�ȡ���������µ�������곬����Χ����

	
	auto tmp_value = map->getPropertiesForGID(gid);//����gid�������ֵ䣨����ֵ�ɣ�
	if (tmp_value.getType() != Value::Type::MAP){
		return;
	}
	auto tileTemp = map->getPropertiesForGID(gid).asValueMap();//��tmp_valueһ����������valuemap���͵ġ�
	int MatrixIndex = static_cast<int>(matrixCoord.y*MAP_WIDTH + matrixCoord.x);//����

	int TouchValue;
	if (tileTemp.empty()){
		TouchValue = 0;
	}
	else
	{
		TouchValue = tileTemp.at("canTouch").asInt();
		log("TouchValue=%d", TouchValue);
	}

	//����Ϊ����ת�������Ǽ������ʱ���Ƿ�����panle�ϣ�����ǣ�����ӣ�������ǣ�����ʾno.png��
	//��Ҫ��һ��
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
		//��Ӣ�۶�����̨ 
		 clickBorder->bindHero(hero);


		type = HeroType::ANOTHER;
		chooseTowerpanel->setChooseHeroType(type);
		this->removeChild(chooseTowerpanel);
		chooseTowerpanel = NULL;
	}
}
*/