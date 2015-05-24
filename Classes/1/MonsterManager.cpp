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
    /* �������� */
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

//���ﲻ֪��Ϊʲô��һ�ŵ�������������ͳ���
void MonsterManager::createMonsters(int iCurLevel, TollgateMapLayer* layer) {
    /* ���ع���������� */
    std::string sMonsterPosPath = StringUtils::format("tollgate/monsterPos_level_%d.plist", iCurLevel);
    auto posList = PosLoadUtil::getInstance()->loadPosWithFile(sMonsterPosPath.c_str(), enMonsterPos, this, 10, false);
    m_monsterPosList.pushBack(posList);


    /* ��ȡ�����ļ� */
    std::string sMonsterConfPath = StringUtils::format("tollgate/monster_level_%d.plist", iCurLevel);

    ValueMap fileDataMap = FileUtils::getInstance()->getValueMapFromFile(sMonsterConfPath.c_str());

    int size = fileDataMap.size();
    for (int i = 1; i <= size; i++){
        Value value = fileDataMap.at(StringUtils::toString(i));
        ValueMap data = value.asValueMap();

        /* �ӹ�����������ļ���ȡ����ID�ͳ���ʱ�䣬�������й��ﵽδ�����б� */
        int id = data["id"].asInt();
        float fShowTime = data["showTime"].asFloat();

        auto monster = Monster::createFromCsvFileByID(id,layer);
        monster->setfShowTime(fShowTime);
  
        /* ���ù��ﾫ�� */
      //  std::string spPath = StringUtils::format("sprite/monster/monster_%d.png", monster->getiModelID());
       // monster->bindSprite(Sprite::create(spPath.c_str()));
		monster->setVisible(false);
		/* ��ȡ����ĵ�һ������� */
		auto monsterFirstPos = getMonsterStartPos();

	//	monster->setPosition(monsterFirstPos->getPos());
	
        /* ���������󵽹����б� */
        m_monsterList.pushBack(monster);

		//��ӵ�ȫ�ֱ�����
		GlobalClient::getInstance()->MonsterVector.pushBack(monster);

        /* ����������δ���������б� */
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

    /* ��ʼ����Ƿ����¹������ */
    this->schedule(schedule_selector(MonsterManager::showMonster));

    /* ����Ƿ��й������� */
    this->schedule(schedule_selector(MonsterManager::logic));
}

void MonsterManager::showMonster(float dt) {
    int iNotShowMonsterCount = m_notShowMonsterList.size();

    if (iNotShowMonsterCount > 0) {
        m_fShowTimeCount += dt;
    }

    Vector<Monster*> monsterWantToDelete;
    for (auto monster: m_notShowMonsterList) {
        /* ʱ��ﵽ����ĳ���ʱ�䣬�ù������ */
        if (m_fShowTimeCount >= monster->getfShowTime()) {
            /* ��ӹ��ﵽɾ���б�������Ĺ���Ҫ��δ�����б���ɾ�� */
            monsterWantToDelete.pushBack(monster);
			//monster->setVisible(false);
			// ��ȡ����ĵ�һ������� 
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
			/* �ù��ﰴ��ָ���������� */
			monster->moveByPosList(m_monsterPosList);

		
           

         
        }
    }

    /* ɾ���Ѿ������Ĺ��� */
    for (auto monster : monsterWantToDelete) {
        m_notShowMonsterList.eraseObject(monster);
    }

    /* �������������ı���Ϣ */
    int iMonsterNumChange = -monsterWantToDelete.size();
    NOTIFY->postNotification("MonsterNumChange", (Ref*)iMonsterNumChange);
}

void MonsterManager::logic(float dt) {
    Vector<Monster*> monsterWantToDelete;
    for (auto monster : m_monsterList) {
        /* ���б���ɾ���Ѿ�����Ŀ�ĵصĹ���ȼ�¼����ɾ�� */
        if (monster->isMoveEnd() == true) {
           monsterWantToDelete.pushBack(monster);

        }
        /* ���б���ɾ���Ѿ������Ĺ���ȼ�¼����ɾ�� */
        else if (monster->isDead() == true) {
            monsterWantToDelete.pushBack(monster);
        }
    }

    /* ��ʽɾ���Ĺ��� */
    for (auto monster : monsterWantToDelete) {
       
        m_monsterList.eraseObject(monster);
		
		//�������ߵ���Ŀ�ĵش�ȫ�ֱ������Ƴ���
		GlobalClient::getInstance()->MonsterVector.eraseObject(monster);
		
		
		monster->removeFromParent();

    }

    if (m_monsterList.size() == 0) {
        /* ��������Ϊ0����������ȫ����Ϣ */
        NOTIFY->postNotification("AllMonsterDead");
    }
}