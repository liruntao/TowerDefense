#include "MonsterManager.h"
#include "EnumPosType.h"
#include "PosLoadUtil.h"
#include "Monster.h"
#include "PosBase.h"
#include "MonsterPos.h"
#include "GlobalDefine.h"
#include "GlobalClient.h"

//static const int DRAG_BODYS_TAG = 0x80;

MonsterManager::MonsterManager() {
    m_fShowTimeCount = 0;
}

MonsterManager::~MonsterManager() {
}

MonsterManager* MonsterManager::createWithLevel(int iCurLevel, TollgateMapLayer*layer) {
    MonsterManager* monsterMgr = new MonsterManager();

	if (monsterMgr && monsterMgr->initWithLevel(iCurLevel, layer)) {
        monsterMgr->autorelease();
    }
    else {
        CC_SAFE_DELETE(monsterMgr);
    }

    return monsterMgr;
}

bool MonsterManager::initWithLevel(int iCurLevel, TollgateMapLayer*layer) {
    /* 创建怪物 */
    createMonsters(iCurLevel,layer);
	_layer = layer;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("CatMaze.plist");
    return true;
}
int MonsterManager::getNotShowMonsterCount() {
    return m_notShowMonsterList.size();
}
Vector<Monster*> MonsterManager::getMonsterList() {
    return m_monsterList;
}
MonsterPos* MonsterManager::getMonsterStartPos() {
    return (MonsterPos*)m_monsterPosList.at(0);
}
MonsterPos* MonsterManager::getMonsterEndPos() {
    return (MonsterPos*)m_monsterPosList.at(m_monsterPosList.size() - 1);
}

//这里不知道为什么，一放到后面设置坐标就出错。
void MonsterManager::createMonsters(int iCurLevel, TollgateMapLayer* layer) {
    /* 加载怪物坐标对象 */
    std::string sMonsterPosPath = StringUtils::format("tollgate/monsterPos_level_%d.plist", iCurLevel);
    auto posList = PosLoadUtil::getInstance()->loadPosWithFile(sMonsterPosPath.c_str(), enMonsterPos, this, 10, false);
    m_monsterPosList.pushBack(posList);


    /* 读取配置文件 */
    std::string sMonsterConfPath = StringUtils::format("tollgate/monster_level_%d.plist", iCurLevel);

    ValueMap fileDataMap = FileUtils::getInstance()->getValueMapFromFile(sMonsterConfPath.c_str());

    int size = fileDataMap.size();
    for (int i = 1; i <= size; i++){
        Value value = fileDataMap.at(StringUtils::toString(i));
        ValueMap data = value.asValueMap();

        /* 从怪物出场配置文件读取怪物ID和出场时间，保存所有怪物到未出场列表 */
        int id = data["id"].asInt();
        float fShowTime = data["showTime"].asFloat();

        auto monster = Monster::createFromCsvFileByID(id,layer);
        monster->setfShowTime(fShowTime);
  
        /* 设置怪物精灵 */
      //  std::string spPath = StringUtils::format("sprite/monster/monster_%d.png", monster->getiModelID());
       // monster->bindSprite(Sprite::create(spPath.c_str()));
		monster->setVisible(false);
		/* 获取怪物的第一个坐标点 */
		auto monsterFirstPos = getMonsterStartPos();

	//	monster->setPosition(monsterFirstPos->getPos());
	
        /* 保存怪物对象到怪物列表 */
        m_monsterList.pushBack(monster);

		//添加到全局变量中
		GlobalClient::getInstance()->MonsterVector.pushBack(monster);

        /* 保存怪物对象到未出场怪物列表 */
        m_notShowMonsterList.pushBack(monster);
		
	

		auto size = monster->getArmature()->getContentSize();
		//	hero->setAnchorPoint(Point::ZERO);
		//	hero->setAnchorPoint(Point(0.5, 0.5));
	//	Point pos = monsterFirstPos->getPos();
	//	monster->setPosition(pos);
		//	hero->setPosition(Point(pos.x+size.width/2,pos.y+size.height/2));
	//	monster->setAnchorPoint(Point(0.5, 0.5));
		auto physicsBody = PhysicsBody::createBox(size,PhysicsMaterial(0.1f,1.0f,1.0f));
		physicsBody->setDynamic(true);
	//	physicsBody->setCollisionBitmask(0x01);
	//	physicsBody->setTag(DRAG_BODYS_TAG);

//		physicsBody->setContactTestBitmask(0xFFFFFFFF);

		physicsBody->setCategoryBitmask(MONSTERBULLET_CATEGORY_BITMASK);    // 1000
		physicsBody->setContactTestBitmask(MONSTERBULLET_CONTACTTEST_BITMASK); // 0010
		physicsBody->setCollisionBitmask(MONSTERBULLET_COLLISION_BITMASK);   // 0001


		physicsBody->setGravityEnable(false);
		monster->setPhysicsBody(physicsBody);

		//monster->setPhysicsBody(physicsBody);
		monster->setTag(MONSTERTAG);
        this->addChild(monster);

    }

    /* 开始检查是否有新怪物出场 */
    this->schedule(schedule_selector(MonsterManager::showMonster));

    /* 检查是否有怪物死亡 */
    this->schedule(schedule_selector(MonsterManager::logic));
}

void MonsterManager::showMonster(float dt) {
    int iNotShowMonsterCount = m_notShowMonsterList.size();

    if (iNotShowMonsterCount > 0) {
        m_fShowTimeCount += dt;
    }

    Vector<Monster*> monsterWantToDelete;
    for (auto monster: m_notShowMonsterList) {
        /* 时间达到怪物的出场时间，让怪物出场 */
        if (m_fShowTimeCount >= monster->getfShowTime()) {
            /* 添加怪物到删除列表，出场后的怪物要出未出场列表中删除 */
            monsterWantToDelete.pushBack(monster);
			//monster->setVisible(false);
			// 获取怪物的第一个坐标点 
			/*
			auto monsterFirstPos = getMonsterStartPos();
			auto pos = monsterFirstPos->getPos();
			monster->setPosition(pos);*/


			monster->setVisible(true);
			//Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());
			//monster->setPosition(monster->getParent()->convertToNodeSpace(heroWorldPos));
			/*
			auto size = monster->getArmature()->getContentSize();

			auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(0.1f, 1.0f, 1.0f));
			physicsBody->setDynamic(true);
			physicsBody->setCategoryBitmask(MONSTERBULLET_CATEGORY_BITMASK);    // 1000
			physicsBody->setContactTestBitmask(MONSTERBULLET_CONTACTTEST_BITMASK); // 0010
			physicsBody->setCollisionBitmask(MONSTERBULLET_COLLISION_BITMASK);   // 0001


			physicsBody->setGravityEnable(false);
			monster->setPhysicsBody(physicsBody);*/
			/* 让怪物按照指定坐标行走 */
			monster->moveByPosList(m_monsterPosList);

		
           

         
        }
    }

    /* 删除已经出场的怪物 */
    for (auto monster : monsterWantToDelete) {
        m_notShowMonsterList.eraseObject(monster);
    }

    /* 发布怪物数量改变消息 */
    int iMonsterNumChange = -monsterWantToDelete.size();
    NOTIFY->postNotification("MonsterNumChange", (Ref*)iMonsterNumChange);
}

void MonsterManager::logic(float dt) {
    Vector<Monster*> monsterWantToDelete;
    for (auto monster : m_monsterList) {
        /* 从列表中删除已经到达目的地的怪物，先记录，后删除 */
        if (monster->isMoveEnd() == true) {
           monsterWantToDelete.pushBack(monster);

        }
        /* 从列表中删除已经死亡的怪物，先记录，后删除 */
        else if (monster->isDead() == true) {
            monsterWantToDelete.pushBack(monster);
        }
    }

    /* 正式删除的怪物 */
    for (auto monster : monsterWantToDelete) {
       
        m_monsterList.eraseObject(monster);
		
		//死亡或者到达目的地从全局变量中移除。
		GlobalClient::getInstance()->MonsterVector.eraseObject(monster);
		
		
		monster->removeFromParent();

    }

    if (m_monsterList.size() == 0) {
        /* 怪物数量为0，发布怪物全灭消息 */
        NOTIFY->postNotification("AllMonsterDead");
    }
}