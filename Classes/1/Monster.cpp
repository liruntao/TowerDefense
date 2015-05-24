#include "Monster.h"
#include "CsvUtil.h"
#include "GlobalPath.h"
#include "PosBase.h"

//#include "ControllerSimpleMove.h"

#include "EnumMonsterPropConfType.h"
#include "GlobalDefine.h"


Monster::Monster() {
    m_moveController = NULL;
    m_iSpeed = MONSTER_SPEED_INIT;
    m_isMoveEnd = false;
	m_armature = NULL;
	m_isDead = false;
	
}

Monster::~Monster() {
	
}

bool Monster::init() {
	//m_moveController = ControllerSimpleMove::create(this);
	

    return true;
}

Monster* Monster::createFromCsvFileByID(int iMonsterID, TollgateMapLayer* layer) {
    Monster* monster = new Monster();
	//_layer = layer;

    if (monster && monster->initFromCsvFileByID(iMonsterID,layer)) {
        monster->autorelease();
    }
    else {
        CC_SAFE_DELETE(monster);
    }
    return monster;
}

bool Monster::initFromCsvFileByID(int iMonsterID,TollgateMapLayer* layer) {
    bool bRet = false;

    do {
        CsvUtil* csvUtil = CsvUtil::getInstance();

        std::string sMonsterID = StringUtils::format("%d", iMonsterID);

	
		m_isDead = false;
        /* 寻找ID所在的行 */
        int iLine = csvUtil->findValueInWithLine(sMonsterID.c_str(),
            enMonsterPropConf_ID, PATH_CSV_MONSTER_CONF);

        CC_BREAK_IF(iLine < 0);

        setID(iMonsterID);
        setiLevel(csvUtil->getInt(iLine, enMonsterPropConf_Level, PATH_CSV_MONSTER_CONF));
        setiModelID(csvUtil->getInt(iLine, enMonsterPropConf_ModelID, PATH_CSV_MONSTER_CONF));
		setiBaseAtk(csvUtil->getInt(iLine, enMonsterPropConf_Atk, PATH_CSV_MONSTER_CONF));
		setiAtkRange(csvUtil->getInt(iLine, enMonsterPropConf_AtkRange, PATH_CSV_MONSTER_CONF));
		setiAtkSpeed(csvUtil->getInt(iLine, enMonsterPropCOnf_AtkSpeed, PATH_CSV_MONSTER_CONF));

        setiDefense(csvUtil->getInt(iLine, enMonsterPropConf_Defense, PATH_CSV_MONSTER_CONF));
        setiHP(csvUtil->getInt(iLine, enMonsterPropConf_Hp, PATH_CSV_MONSTER_CONF));
        setiSpeed(csvUtil->getInt(iLine, enMonsterPropConf_Speed, PATH_CSV_MONSTER_CONF));
		setMetalAtk(csvUtil->getInt(iLine, enMonsterPropConf_MetalAtk, PATH_CSV_MONSTER_CONF));
		setMetalDefense(csvUtil->getInt(iLine, enMonsterPropConf_MetalDefense, PATH_CSV_MONSTER_CONF));
		setWoodAtk(csvUtil->getInt(iLine, enMonsterPropConf_WoodAtk, PATH_CSV_MONSTER_CONF));
		setWoodDefense(csvUtil->getInt(iLine, enMonsterPropConf_WoodDefense, PATH_CSV_MONSTER_CONF));
		setWaterAtk(csvUtil->getInt(iLine, enMonsterPropConf_WaterAtk, PATH_CSV_MONSTER_CONF));
		setWaterDefense(csvUtil->getInt(iLine, enMonsterPropConf_WaterDefense, PATH_CSV_MONSTER_CONF));
		setFireAtk(csvUtil->getInt(iLine, enMonsterPropConf_FireAtk, PATH_CSV_MONSTER_CONF));
		setFireDefense(csvUtil->getInt(iLine, enMonsterPropConf_FireDefense, PATH_CSV_MONSTER_CONF));
		setEarthAtk(csvUtil->getInt(iLine, enMonsterPropConf_EarthAtk, PATH_CSV_MONSTER_CONF));
		setEarthDefense(csvUtil->getInt(iLine, enMonsterPropConf_EarthDefense, PATH_CSV_MONSTER_CONF));

		initValueMapTheFiveLineProperties();


		ArmatureDataManager::getInstance()->addArmatureFileInfo("armatureResour/monster_1.ExportJson");
		ArmatureDataManager::getInstance()->addArmatureFileInfo("armatureResour/monster_2.ExportJson");
	//	ArmatureDataManager::getInstance()->addArmatureFileInfo("armatureResour/monster_3.ExportJson");
	//	ArmatureDataManager::getInstance()->addArmatureFileInfo("armatureResour/monster_4.ExportJson");
	//	ArmatureDataManager::getInstance()->addArmatureFileInfo("armatureResour/monster_5.ExportJson");
	//	std::string spPath1 = StringUtils::format("sprite/monster/monster_%d.png", this->getiModelID());
	//	auto sprite = Sprite::create(spPath1.c_str());
	//	this->bindSprite(sprite);


		//在这里设计怪物模型，关键是如果用骨骼动画的话，先不管开销，以后做动画的时候就简单多了，无非调整play的种类即可
		std::string spPath = StringUtils::format("monster_%d", this->getiModelID());

		Armature *armature = nullptr;
		armature = Armature::create(spPath.c_str());
		//	armature = Armature::create("arrowTower");
		//armature->getAnimation()->play("Fire");
		armature->setScaleX(-1);
		//armature->setScaleY(0.05f);
		armature->setPosition(Point(0, 0));
		
		armature->getAnimation()->play("stay");
//		armature->setAnchorPoint(Point(0, 0));
		bindArmature(armature);
		/*
		//this->addChild(armature);
		auto size = this->getArmature()->getContentSize();
		//	hero->setAnchorPoint(Point::ZERO);
		//	hero->setAnchorPoint(Point(0.5, 0.5));
		//	Point pos = monsterFirstPos->getPos();
		//	monster->setPosition(pos);
		//	hero->setPosition(Point(pos.x+size.width/2,pos.y+size.height/2));
		//	monster->setAnchorPoint(Point(0.5, 0.5));
		auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(0.1f, 1.0f, 1.0f));
		physicsBody->setDynamic(true);
		//	physicsBody->setCollisionBitmask(0x01);
		//	physicsBody->setTag(DRAG_BODYS_TAG);

		//		physicsBody->setContactTestBitmask(0xFFFFFFFF);

		physicsBody->setCategoryBitmask(MONSTERBULLET_CATEGORY_BITMASK);    // 1000
		physicsBody->setContactTestBitmask(MONSTERBULLET_CONTACTTEST_BITMASK); // 0010
		physicsBody->setCollisionBitmask(MONSTERBULLET_COLLISION_BITMASK);   // 0001


		physicsBody->setGravityEnable(false);

		//monster->setPhysicsBody(physicsBody);
		this->setPhysicsBody(physicsBody);
		this->setTag(MONSTERTAG); 
*/


		m_moveController = ControllerAMove::createWithLayer(this, layer);
		m_moveController->setiSpeed(this->getiSpeed());
		this->addChild(m_moveController);

		/* 绑定移动结束回调函数 */
		m_moveController->bindMoveEndFunc([&](){
			/* 发布魔力值改变消息 */
			int iMagicChange = -getiLevel() * 10;
			NOTIFY->postNotification("MagicChange", (Ref*)iMagicChange);

			m_isMoveEnd = true;
		});



        CC_BREAK_IF(!init());

        bRet = true;
    } while (0);

    return bRet;
}



void Monster::moveByPosList(Vector<PosBase*> posList) {
	//this->stopAllActions();

	if (posList.size() < 1) {
        return;
    }

    m_moveController->moveByPosList(posList, 2, getiSpeed());
}

void Monster::onDead() {
	m_isDead = true;
	
	log("1");
    /* 发布塔魂增加消息 */
    int iTowerSoulNumChange = 3 * getiLevel();


    NOTIFY->postNotification("TowerSoulChange", (Ref*)iTowerSoulNumChange);
	NOTIFY->postNotification("MonsterDeadBody", (Ref*)this);
	//this->removeFromParent();



}
void Monster::onBindSprite() {

    /* 创建血量条控件 */
    auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("HpBar/HpBar_1.ExportJson");
    this->addChild(UI);

    /* 设置坐标 */
    Size size = this->getContentSize();
    UI->setPosition(Point(0, size.height * 0.5f));

    /* 获取进度条控件 */
    m_hpBar = (LoadingBar*)Helper::seekWidgetByName(UI, "hpbar");
    m_hpBar->setPercent(100);

    /* 记录初始血量 */
    m_iMaxHP = getiHP();
	

}

void Monster::onHurt(int iHurtValue) {
    m_hpBar->setPercent(getiHP() / (float)m_iMaxHP * 100);
}

ControllerAMove* Monster::getMoveController(){
	return m_moveController;
}

void Monster::initValueMapTheFiveLineProperties(){
	theFiveLineProperties["metalDefense"] = this->getMetalDefense();
}
