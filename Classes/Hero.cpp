#include "Hero.h"
#include "CsvUtil.h"
#include "GlobalPath.h"
//#include "CCString.h"
#include "EnumHeroPropConfType.h"
#include "TollgateMapLayer.h"
#include "TollgateScene.h"
#include "Monster.h"
#include "BulletManager.h"
#include "BulletBase.h"
#include "TollgateDataLayer.h"
#include "GlobalDefine.h"
#include "GlobalClient.h"
#include "BulletMulti.h"
#include "BulletRiprap.h"
#include "BulletNormal.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;
/*
这个检测，当然首先是创建一个hero，bindsprite，读取根据iHeroId读取csv里的数据，set到这个英雄的属性里，创建一个sprite，并绑定到hero上
那个heromanager里有个logic函数，一直调用checkatkmonster并传进来一个monsterlist（一开始为空）（其实没啥用，主要是为了实时删除和添加
但是为什么不在hero里创建呢？可能是怕在heromanager里用到？monsterlist）tBorder->getHero()->checkAtkMonster(dt, monsterList)
检查m_atkmonster是否为空（刚开始肯定为空，是现在正在攻击的目标，这个逻辑就是如果目标没有离开，就一直攻击它，知道死亡或者离开（这里也可以添加策略，可以选择打离自己最远的等））
如果非空，则判断它是否已经死亡，如果死亡，则从列表里移除，并且m_atkmonster置空，return，结束这个循环，
如果没有死亡，则调用atk函数，攻击它（攻击一次），然后判断是否离开目标，如果离开，则将m_atkmonster置空，并把它从list里删除（不删除，这个monster是从外边传进来的，除非死亡，否则不删除）。
如果没有m_atkmonster，则chooseaim，选一个，



*/
Hero::Hero() {
    m_atkMonster = NULL;
    m_isAtkCoolDown = false;
	m_bulletType = 1;
}

Hero::~Hero() {
}

Hero* Hero::create(Sprite* sprite) {
    Hero* hero = new Hero();
    if (hero && hero->init(sprite)) {
        hero->autorelease();
    }
    else {
        CC_SAFE_DELETE(hero);
    }
    return hero;
}

bool Hero::init(Sprite* sprite) {
    bool bRet = false;
    do {
        CC_BREAK_IF(!sprite);
        bindSprite(sprite);
        bRet = true;
    } while (0);

    return bRet;
}

Hero* Hero::createFromCsvFileByID(int iHeroID,int iBulletID) {
    Hero* hero = new Hero();

	//添加子弹种类，这个以后选择子弹的时候就从这里选择。
    if (hero && hero->initFromCsvFileByID(iHeroID,iBulletID)) {
        hero->autorelease();
    }
    else {
        CC_SAFE_DELETE(hero);
    }
    return hero;
}


//没有绑定sprite和bulletmgr吧？？
bool Hero::initFromCsvFileByID(int iHeroID,int iBulletID) {
    bool bRet = false;
	m_bulletType = iBulletID;
	m_bullet1 = BulletMulti::create();
	this->addChild(m_bullet1);


    do {
        CsvUtil* csvUtil = CsvUtil::getInstance();

        std::string sHeroID = StringUtils::toString(iHeroID);

        /* 寻找ID所在的行 */
        int iLine = csvUtil->findValueInWithLine(
            sHeroID.c_str(), enHeroPropConf_ID, PATH_CSV_HERO_CONF);

        CC_BREAK_IF(iLine < 0);

        setID(iHeroID);
        setiModelID(csvUtil->getInt(iLine, enHeroPropConf_ModelID, PATH_CSV_HERO_CONF));
        setiBaseAtk(csvUtil->getInt(iLine, enHeroPropConf_BaseAtk, PATH_CSV_HERO_CONF));
        setiCurAtk(getiBaseAtk());
        setiAtkSpeed(csvUtil->getInt(iLine, enHeroPropConf_AtkSpeed, PATH_CSV_HERO_CONF));
        setiAtkRange(csvUtil->getInt(iLine, enHeroPropConf_AtkRange, PATH_CSV_HERO_CONF));
		setiDefense(csvUtil->getInt(iLine, enHeroPropConf_Defense, PATH_CSV_HERO_CONF));
		setiHP(csvUtil->getInt(iLine, enHeroPropConf_Hp, PATH_CSV_HERO_CONF));
        setiUpgradeCostBase(csvUtil->getInt(iLine, enHeroPropConf_UpgradeCostBase, PATH_CSV_HERO_CONF));
        setfUpgradeAtkBase(csvUtil->getFloat(iLine, enHeroPropConf_UpgradeAtkBase, PATH_CSV_HERO_CONF));

        Sprite* sprite = Sprite::create(StringUtils::format("sprite/hero/hero_%d.png", iHeroID).c_str());

		ArmatureDataManager::getInstance()->addArmatureFileInfo("armatureResour/monster_2.ExportJson");

		Armature *armature = nullptr;
		armature = Armature::create("monster_2");
		armature->getAnimation()->playWithIndex(0);
		armature->setScale(0.2f);

		armature->setPosition(Point(0, 0));

		addChild(armature, 100);
		//bindSprite(sprite);
        CC_BREAK_IF(!sprite);

        CC_BREAK_IF(!init(sprite));

        bRet = true;
    } while (0);

    return bRet;
}

//为编辑界面准备
Hero* Hero::createForSelect(int iHeroID, int iBulletID) {
	Hero* hero = new Hero();

	//添加子弹种类，这个以后选择子弹的时候就从这里选择。
	if (hero && hero->initForSelect(iHeroID, iBulletID)) {
		hero->autorelease();
	}
	else {
		CC_SAFE_DELETE(hero);
	}
	return hero;
}


//没有绑定sprite和bulletmgr吧？？
bool Hero::initForSelect(int iHeroID, int iBulletID) {
	bool bRet = false;
	m_bulletType = iBulletID;
	
	

	do {
		CsvUtil* csvUtil = CsvUtil::getInstance();

		std::string sHeroID = StringUtils::toString(iHeroID);

		/* 寻找ID所在的行 */
		int iLine = csvUtil->findValueInWithLine(
			sHeroID.c_str(), enHeroPropConf_ID, PATH_CSV_HERO_CONF);

		CC_BREAK_IF(iLine < 0);
		this->getParent();
		setID(iHeroID);
		setiModelID(csvUtil->getInt(iLine, enHeroPropConf_ModelID, PATH_CSV_HERO_CONF));
		setiBaseAtk(csvUtil->getInt(iLine, enHeroPropConf_BaseAtk, PATH_CSV_HERO_CONF));
		setiCurAtk(getiBaseAtk());
		setiAtkSpeed(csvUtil->getInt(iLine, enHeroPropConf_AtkSpeed, PATH_CSV_HERO_CONF));
		setiAtkRange(csvUtil->getInt(iLine, enHeroPropConf_AtkRange, PATH_CSV_HERO_CONF));
		setiDefense(csvUtil->getInt(iLine, enHeroPropConf_Defense, PATH_CSV_HERO_CONF));
		setiHP(csvUtil->getInt(iLine, enHeroPropConf_Hp, PATH_CSV_HERO_CONF));
		setiUpgradeCostBase(csvUtil->getInt(iLine, enHeroPropConf_UpgradeCostBase, PATH_CSV_HERO_CONF));
		setfUpgradeAtkBase(csvUtil->getFloat(iLine, enHeroPropConf_UpgradeAtkBase, PATH_CSV_HERO_CONF));

		Sprite* sprite = Sprite::create(StringUtils::format("sprite/hero/hero_%d.png", iHeroID).c_str());
		//ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("armatureResour/arrowTower.ExportJson", this, SEL_SCHEDULE(Hero::Null));
		ArmatureDataManager::getInstance()->addArmatureFileInfo("armatureResour/monster_1.ExportJson");
		
		Armature *armature = nullptr;
		armature = Armature::create("monster_1");
		armature->getAnimation()->playWithIndex(0);
		armature->setScale(0.2f);
		
		armature->setPosition(Point(0, 0));
	
		addChild(armature,100);

		//这个子弹路径以后再该，其他的路径也一样，尽量都使用格式化路径，这样修改的话只修改一个地方就好
		switch (iBulletID)
		{
		case 1:{
			Sprite* bullet_select = Sprite::create(PATH_BULLET_NOR);
			Size spriteContent = bullet_select->getContentSize();
			bullet_select->setPosition(spriteContent.width, 0);
			this->addChild(bullet_select);
			break;
		}
		case 2:{
			Sprite* bullet_select = Sprite::create(PATH_BULLET_ARR);
			Size spriteContent = bullet_select->getContentSize();
			bullet_select->setPosition(spriteContent.width, 0);
			this->addChild(bullet_select);
			break;}
		case 3:
		{Sprite* bullet_select = Sprite::create(PATH_BULLET_RIPRAP);
		Size spriteContent = bullet_select->getContentSize();
		bullet_select->setPosition(spriteContent.width, 0);
		this->addChild(bullet_select);
		break; }
		default:
			break;
		}
		
	
		//bindSprite(sprite);
		CC_BREAK_IF(!sprite);

		CC_BREAK_IF(!init(sprite));

		bRet = true;
	} while (0);

	return bRet;
}


void Hero::checkAtkMonster(float ft, Vector<Monster*> monsterList) {
    //这里用if不对吧，if不是只执行一次？？
	//就是只执行一次，但是调用这个函数的schedule每隔一段时间就调用一次，因此就使用if，但是这样会不会效率低？
	//这个monsterlist传进来的应该是一个首地址吧，那么就应该没必要eraserobject吧，直接置空就可以？
	if (m_atkMonster != NULL) {
        /* 怪物已死亡 */
        if (m_atkMonster->isDead()) {
            /* 从怪物列表中删除怪物 */
           //应该没必要,改好了试试
			monsterList.eraseObject(m_atkMonster);

            /* 清除锁定的怪物引用 */
            m_atkMonster = NULL;
            return;
        }
        /* 攻击冷却结束，可以攻击 */
        if (m_isAtkCoolDown == false) {
            atk(ft);
        }

        /* 判断怪物是否离开攻击范围 */
        checkAimIsOutOfRange(monsterList);
    }
    else {
        /* 选择一个进入攻击范围的怪物 */
        chooseAim(monsterList);
    }
}

void Hero::chooseAim(Vector<Monster*> monsterList) {
    for (auto monster : monsterList) {
        if (monster->isVisible() && isInAtkRange(monster->getPosition())) {
            chooseAtkMonster(monster);//按添加到monsterlist里的顺序，第一个存在的，就置为目标
            log("InAtkRange!!!");
            break;
        }
    }
}
//就是取hero的坐标，取monster的坐标，然后让length=这两个坐标之间平方距离，如果length《=攻击距离的平方，则在范围内。
bool Hero::isInAtkRange(Point pos) {//这个pos是这个monster的position，get的都是opengl的position
    int iDoubleAtkRange = getiAtkRange();	//攻击范围，用平方一个是计算快，一个是忽略正负，还有就是避免开方出现误差？
    Point heroPos = getPosition();//get这个hero的position

    float length = pos.getDistanceSq(heroPos);
    if (length <= iDoubleAtkRange * iDoubleAtkRange) {
        return true;
    }
    return false;   
}

void Hero::chooseAtkMonster(Monster* monster) {
    m_atkMonster = monster;
}

void Hero::atk(float ft) {
	

	auto rotateVector = m_atkMonster->getPosition() - this->getPosition();
	auto rotateRadians = rotateVector.getAngle();
	auto rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians);

	auto speed = 0.5 / M_PI;
	auto rotateDuration = fabs(rotateRadians * speed);

	//以后等转弯以后再发射子弹。
	this->getSprite()->runAction(Sequence::create(RotateTo::create(rotateDuration, rotateDegrees),NULL));
//	this->getSprite()->runAction(Sequence::create(RotateTo::create(rotateDuration, rotateDegrees),
//		CallFunc::create(CC_CALLBACK_0(Hero::createBullet6, this, aim)),
//		NULL));


	switch (m_bulletType){
	case 1:{
			   BulletNormal* m_bullet = BulletNormal::create();
			   auto size = m_bullet->getSprite()->getContentSize();
			   auto physicsBody = PhysicsBody::createBox(size);
			   physicsBody->setDynamic(true);
			   physicsBody->setCategoryBitmask(HEROBULLET_CATEGORY_BITMASK);
			   physicsBody->setCollisionBitmask(HEROBULLET_COLLISION_BITMASK);
			   physicsBody->setContactTestBitmask(HEROBULLET_CONTACTTEST_BITMASK);
			   m_bullet->setPhysicsBody(physicsBody);


			   this->addChild(m_bullet);
			   m_bullet->setiAtkValue(getiCurAtk());
			   m_bullet->onLockAim(m_atkMonster);
			   m_bullet->setTag(BULLET_ONETAG);

			   break;
	}
	case 2:{
			  
			   m_bullet1->setiAtkValue(getiCurAtk());
			   m_bullet1->onLockAim(m_atkMonster);

			   break;
	}
	case 3:{
			   BulletRiprap* m_bullet = BulletRiprap::create();
			   this->addChild(m_bullet);
			   m_bullet->setiAtkValue(getiCurAtk());
			   m_bullet->onLockAim(m_atkMonster);
			   break;
	}

}
	m_isAtkCoolDown = true;
	this->scheduleOnce(schedule_selector(Hero::atkCollDownEnd), getiAtkSpeed() / 1000.0f);


   /*  if (bullet != NULL) {
        根据英雄情况设置子弹属性，锁定攻击目标 
		//这里为什么要这么处理，直接取得bullet的世界坐标，取monster的世界坐标不就行了？
      
		//这个坐标转化没搞明白
		Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());
	   bullet->setPosition(bullet->getParent()->convertToNodeSpace(heroWorldPos));
		
		bullet->setiAtkValue(getiCurAtk());//实时获取攻击力
       // bullet->lockAim(m_atkMonster);

		bullet6->setiAtkValue(getiCurAtk());
		bullet6->setPosition(bullet6->getParent()->convertToNodeSpace(heroWorldPos));
		bullet6->lockAim(m_atkMonster);
		bullet6->onLockAim(m_atkMonster);
		

		//auto bulletVector = instance->BulletVector;
		GlobalClient::getInstance()->BulletVector.pushBack(bullet);

        // 标记攻击冷却 
        m_isAtkCoolDown = true;

        // 英雄攻击有间隔时间，指定若干秒后恢复攻击 
        this->scheduleOnce(schedule_selector(Hero::atkCollDownEnd), getiAtkSpeed() / 1000.0f);
    }*/
}

void Hero::atkCollDownEnd(float dt) {
    m_isAtkCoolDown = false;
}

void Hero::checkAimIsOutOfRange(Vector<Monster*> monsterList) {
    if (m_atkMonster != NULL) {
        if (isInAtkRange(m_atkMonster->getPosition()) == false) {
            missAtkMonster();
        }
    }
}
void Hero::missAtkMonster() {
    log("Out of Range");
    m_atkMonster = NULL;
}

void Hero::upgrade(){
    Sprite* sprite = getSprite();
    if (sprite == NULL || m_iLevel >= 4) {
        return;
    }

    /* 判断塔魂是否足够
	获得现在正在运行的场景的tag_data_layer的值
	*/
	//获得现在正在运行的场景的tag_data_layer的widget

    auto dataLayer = (TollgateDataLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_DATA_LAYER);

    int iCurMagicNum = dataLayer->getiTowerSoulNum();

    int iCostTowerSoul = m_iUpgradeCostBase * m_iLevel;
    if (iCurMagicNum < iCostTowerSoul) {
        return;
    }

    /* 发布消息，扣除塔魂 */
    NOTIFY->postNotification("TowerSoulChange", (Ref*)-iCostTowerSoul);

    /* 增加等级 */
    m_iLevel++;

    /* 英雄等级特效 */
    if (m_iLevel == 2) {
        Sprite* heroTop1 = Sprite::create("sprite/hero/hero_top_1.png");
        this->addChild(heroTop1);
    }
    if (m_iLevel == 3) {
        Sprite* heroTop2 = Sprite::create("sprite/hero/hero_top_2.png");
        this->addChild(heroTop2);

        auto rotateBy = RotateBy::create(25.0f, 360, 360);
        auto repeat = RepeatForever::create(rotateBy);
        heroTop2->runAction(repeat);
    }
    if (m_iLevel == 4) {
        Sprite* heroTop3 = Sprite::create("sprite/hero/hero_top_3.png");
        this->addChild(heroTop3);

        auto rotateBy = RotateBy::create(10.0f, 360, 360);
        auto repeat = RepeatForever::create(rotateBy);
        heroTop3->runAction(repeat);
    }

    /* 增加英雄攻击力 */
    setiBaseAtk(getiBaseAtk() * m_fUpgradeAtkBase);
    setiCurAtk(getiBaseAtk());
}

void Hero::Null(){

}