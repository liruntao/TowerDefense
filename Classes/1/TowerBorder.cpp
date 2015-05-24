#include "TowerBorder.h"
#include "Hero.h"
#include "GlobalDefine.h"

TowerBorder::TowerBorder() {
    m_iLevel = 1;
    m_hero = NULL;
    m_cancelBtn = NULL;
    m_deleteBtn = NULL;
    m_editorBtn = NULL;
    m_isOprBtnsShow = false;
}   
TowerBorder::~TowerBorder() {
}

bool TowerBorder::init() {
    return true;
}

void TowerBorder::upgrade() {
    if (getSprite() != NULL) {
        getSprite()->stopAllActions();
    }

    std::string sFilePath = StringUtils::format("sprite/hero/border_%d.png", m_iLevel);
    Sprite* sprite = Sprite::create(sFilePath.c_str());

    bindSprite(sprite);

    m_iLevel++;

    if (m_iLevel == 2) {

        auto rotateBy = RotateBy::create(25.0f, 360, 360);
        auto repeat = RepeatForever::create(rotateBy);

        sFilePath = StringUtils::format("sprite/hero/magic_border_%d.png", m_iLevel);
        sprite = Sprite::create(sFilePath.c_str());
        sprite->setOpacity(180);
        sprite->runAction(repeat);
        this->addChild(sprite, 10);
    }
}

bool TowerBorder::isClickMe(Point pos) {
    Size size = getSprite()->getContentSize();
    Point borderPos = getPosition();

    Point srcPos = Point(borderPos.x - size.width / 2, borderPos.y + size.height / 2);
    Point destPos = Point(borderPos.x + size.width / 2, borderPos.y - size.height / 2);

    if (pos.x >= srcPos.x && pos.x <= destPos.x && pos.y <= srcPos.y && pos.y >= destPos.y) {
        return true;
    }

    return false;
}

void TowerBorder::bindHero(Hero* hero) {
    m_hero = hero;
//	this->addChild(m_hero);
}

Hero* TowerBorder::getHero() {
    return m_hero;
}

void TowerBorder::deleteHero() {
    if (m_hero != NULL) {
        m_hero->removeFromParent();
    }
}

void TowerBorder::showTowerOprBtns(){
   /* if (m_isOprBtnsShow == true) {
        //已经是显示状态 
        return;
    }*/
    if (m_cancelBtn == NULL) {
        /* 手动创建一个按钮 */
        auto heroOprBtn = Button::create();
        heroOprBtn->loadTextureNormal("button2.png");

        /* 用过clone函数复制三个按钮对象 */
        m_cancelBtn = (Button*)heroOprBtn->clone();
        m_deleteBtn = (Button*)heroOprBtn->clone();
        m_editorBtn = (Button*)heroOprBtn->clone();

        /* 初始化按钮位置 */
        resetOprBtns();

        m_cancelBtn->setTitleText("cancel");
        m_deleteBtn->setTitleText("delete");
        m_editorBtn->setTitleText("editor");

        m_cancelBtn->addTouchEventListener(this, toucheventselector(TowerBorder::cancelClick));
        m_deleteBtn->addTouchEventListener(this, toucheventselector(TowerBorder::deleteClick));
        m_editorBtn->addTouchEventListener(this, toucheventselector(TowerBorder::editorClick));

        this->getParent()->addChild(m_cancelBtn, 999);
        this->getParent()->addChild(m_deleteBtn, 999);
		this->getParent()->addChild(m_editorBtn, 999);
    }

    m_cancelBtn->setEnabled(true);
    m_deleteBtn->setEnabled(true);
	m_editorBtn->setEnabled(true);

    /* 按钮出场特效 */
    m_cancelBtn->runAction(EaseBounceOut::create(MoveBy::create(0.5f, Point(0, 50))));
    m_deleteBtn->runAction(EaseBounceOut::create(MoveBy::create(0.5f, Point(-50, 0))));
    m_editorBtn->runAction(EaseBounceOut::create(MoveBy::create(0.5f, Point(50, 0))));

    m_isOprBtnsShow = true;
}
//点别的地方时要调用它，避免老是显示
void TowerBorder::deleteOprBtns(){
    if (m_cancelBtn != NULL) {
        m_cancelBtn->setEnabled(false);
        m_deleteBtn->setEnabled(false);
        m_editorBtn->setEnabled(false);

        resetOprBtns();
    }

   // m_isOprBtnsShow = false;
	return;

}

void TowerBorder::resetOprBtns(){
    /* 让按钮恢复到中点的位置 
	*/
    Point pos = this->getPosition();
    m_cancelBtn->setPosition(pos);
    m_deleteBtn->setPosition(pos);
    m_editorBtn->setPosition(pos);
}

void TowerBorder::cancelClick(Ref* target, TouchEventType type){
    if (type == TouchEventType::TOUCH_EVENT_ENDED){
        deleteOprBtns();

    }
}

void TowerBorder::deleteClick(Ref* target, TouchEventType type){
    if (type == TouchEventType::TOUCH_EVENT_ENDED){
        //deleteHero();
        //m_hero = NULL;
		deleteOprBtns();

		NOTIFY->postNotification("DeletTowerBord", (Ref*)this);

    }
}

void TowerBorder::editorClick(Ref* target, TouchEventType type){
    if (type == TouchEventType::TOUCH_EVENT_ENDED){
     //   m_hero->upgrade();
		NOTIFY->postNotification("HeroAndBulletTypeChange", (Ref*)this);

        deleteOprBtns();
    }
}

