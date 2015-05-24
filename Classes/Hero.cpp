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
�����⣬��Ȼ�����Ǵ���һ��hero��bindsprite����ȡ����iHeroId��ȡcsv������ݣ�set�����Ӣ�۵����������һ��sprite�����󶨵�hero��
�Ǹ�heromanager���и�logic������һֱ����checkatkmonster��������һ��monsterlist��һ��ʼΪ�գ�����ʵûɶ�ã���Ҫ��Ϊ��ʵʱɾ�������
����Ϊʲô����hero�ﴴ���أ�����������heromanager���õ���monsterlist��tBorder->getHero()->checkAtkMonster(dt, monsterList)
���m_atkmonster�Ƿ�Ϊ�գ��տ�ʼ�϶�Ϊ�գ����������ڹ�����Ŀ�꣬����߼��������Ŀ��û���뿪����һֱ��������֪�����������뿪������Ҳ������Ӳ��ԣ�����ѡ������Լ���Զ�ĵȣ���
����ǿգ����ж����Ƿ��Ѿ��������������������б����Ƴ�������m_atkmonster�ÿգ�return���������ѭ����
���û�������������atk������������������һ�Σ���Ȼ���ж��Ƿ��뿪Ŀ�꣬����뿪����m_atkmonster�ÿգ���������list��ɾ������ɾ�������monster�Ǵ���ߴ������ģ���������������ɾ������
���û��m_atkmonster����chooseaim��ѡһ����



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

	//����ӵ����࣬����Ժ�ѡ���ӵ���ʱ��ʹ�����ѡ��
    if (hero && hero->initFromCsvFileByID(iHeroID,iBulletID)) {
        hero->autorelease();
    }
    else {
        CC_SAFE_DELETE(hero);
    }
    return hero;
}


//û�а�sprite��bulletmgr�ɣ���
bool Hero::initFromCsvFileByID(int iHeroID,int iBulletID) {
    bool bRet = false;
	m_bulletType = iBulletID;
	m_bullet1 = BulletMulti::create();
	this->addChild(m_bullet1);


    do {
        CsvUtil* csvUtil = CsvUtil::getInstance();

        std::string sHeroID = StringUtils::toString(iHeroID);

        /* Ѱ��ID���ڵ��� */
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

//Ϊ�༭����׼��
Hero* Hero::createForSelect(int iHeroID, int iBulletID) {
	Hero* hero = new Hero();

	//����ӵ����࣬����Ժ�ѡ���ӵ���ʱ��ʹ�����ѡ��
	if (hero && hero->initForSelect(iHeroID, iBulletID)) {
		hero->autorelease();
	}
	else {
		CC_SAFE_DELETE(hero);
	}
	return hero;
}


//û�а�sprite��bulletmgr�ɣ���
bool Hero::initForSelect(int iHeroID, int iBulletID) {
	bool bRet = false;
	m_bulletType = iBulletID;
	
	

	do {
		CsvUtil* csvUtil = CsvUtil::getInstance();

		std::string sHeroID = StringUtils::toString(iHeroID);

		/* Ѱ��ID���ڵ��� */
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

		//����ӵ�·���Ժ��ٸã�������·��Ҳһ����������ʹ�ø�ʽ��·���������޸ĵĻ�ֻ�޸�һ���ط��ͺ�
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
    //������if���԰ɣ�if����ִֻ��һ�Σ���
	//����ִֻ��һ�Σ����ǵ������������scheduleÿ��һ��ʱ��͵���һ�Σ���˾�ʹ��if�����������᲻��Ч�ʵͣ�
	//���monsterlist��������Ӧ����һ���׵�ַ�ɣ���ô��Ӧ��û��Ҫeraserobject�ɣ�ֱ���ÿվͿ��ԣ�
	if (m_atkMonster != NULL) {
        /* ���������� */
        if (m_atkMonster->isDead()) {
            /* �ӹ����б���ɾ������ */
           //Ӧ��û��Ҫ,�ĺ�������
			monsterList.eraseObject(m_atkMonster);

            /* ��������Ĺ������� */
            m_atkMonster = NULL;
            return;
        }
        /* ������ȴ���������Թ��� */
        if (m_isAtkCoolDown == false) {
            atk(ft);
        }

        /* �жϹ����Ƿ��뿪������Χ */
        checkAimIsOutOfRange(monsterList);
    }
    else {
        /* ѡ��һ�����빥����Χ�Ĺ��� */
        chooseAim(monsterList);
    }
}

void Hero::chooseAim(Vector<Monster*> monsterList) {
    for (auto monster : monsterList) {
        if (monster->isVisible() && isInAtkRange(monster->getPosition())) {
            chooseAtkMonster(monster);//����ӵ�monsterlist���˳�򣬵�һ�����ڵģ�����ΪĿ��
            log("InAtkRange!!!");
            break;
        }
    }
}
//����ȡhero�����꣬ȡmonster�����꣬Ȼ����length=����������֮��ƽ�����룬���length��=���������ƽ�������ڷ�Χ�ڡ�
bool Hero::isInAtkRange(Point pos) {//���pos�����monster��position��get�Ķ���opengl��position
    int iDoubleAtkRange = getiAtkRange();	//������Χ����ƽ��һ���Ǽ���죬һ���Ǻ������������о��Ǳ��⿪��������
    Point heroPos = getPosition();//get���hero��position

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

	//�Ժ��ת���Ժ��ٷ����ӵ���
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
        ����Ӣ����������ӵ����ԣ���������Ŀ�� 
		//����ΪʲôҪ��ô����ֱ��ȡ��bullet���������꣬ȡmonster���������겻�����ˣ�
      
		//�������ת��û������
		Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());
	   bullet->setPosition(bullet->getParent()->convertToNodeSpace(heroWorldPos));
		
		bullet->setiAtkValue(getiCurAtk());//ʵʱ��ȡ������
       // bullet->lockAim(m_atkMonster);

		bullet6->setiAtkValue(getiCurAtk());
		bullet6->setPosition(bullet6->getParent()->convertToNodeSpace(heroWorldPos));
		bullet6->lockAim(m_atkMonster);
		bullet6->onLockAim(m_atkMonster);
		

		//auto bulletVector = instance->BulletVector;
		GlobalClient::getInstance()->BulletVector.pushBack(bullet);

        // ��ǹ�����ȴ 
        m_isAtkCoolDown = true;

        // Ӣ�۹����м��ʱ�䣬ָ���������ָ����� 
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

    /* �ж������Ƿ��㹻
	��������������еĳ�����tag_data_layer��ֵ
	*/
	//��������������еĳ�����tag_data_layer��widget

    auto dataLayer = (TollgateDataLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_DATA_LAYER);

    int iCurMagicNum = dataLayer->getiTowerSoulNum();

    int iCostTowerSoul = m_iUpgradeCostBase * m_iLevel;
    if (iCurMagicNum < iCostTowerSoul) {
        return;
    }

    /* ������Ϣ���۳����� */
    NOTIFY->postNotification("TowerSoulChange", (Ref*)-iCostTowerSoul);

    /* ���ӵȼ� */
    m_iLevel++;

    /* Ӣ�۵ȼ���Ч */
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

    /* ����Ӣ�۹����� */
    setiBaseAtk(getiBaseAtk() * m_fUpgradeAtkBase);
    setiCurAtk(getiBaseAtk());
}

void Hero::Null(){

}