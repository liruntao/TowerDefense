/*
一些简单的函数。
*/
#include "Entity.h"

Entity::Entity() {
    m_sprite = NULL;
    m_sName = "";
    m_iHP = 1;
    m_iDefense = 1;
    m_isDead = false;
    m_iSpeed = 1;
    m_iLevel = 1;
	cgDefense = 0;
	cgHp = 0;
	cgSpeed = 0;
	bodycgSpeed = 0;
	m_MetalAtk = 0;
	m_MetalDefense = 0;
	m_WoodAtk = 0;
	m_WoodDefense = 0;
	m_WaterAtk = 0;
	m_WaterDefense = 0;
	m_FireAtk = 0;
	m_FireDefense = 0;
	m_EarthAtk = 0;
	m_EarthDefense = 0;
}

Entity::~Entity() {
}

void Entity::bindSprite(Sprite* sprite) {
    if (this->m_sprite != NULL) {
        m_sprite->removeFromParentAndCleanup(true);
    }

    this->m_sprite = sprite;

		
    this->addChild(m_sprite);

    Size size = m_sprite->getContentSize();
   
    onBindSprite();
}

Sprite* Entity::getSprite() {
    return this->m_sprite;
}

void Entity::bindArmature(Armature* armature) {
	if (this->m_armature != NULL) {
		m_armature->removeFromParentAndCleanup(true);
	}

	this->m_armature = armature;
	this->addChild(m_armature);

	Size size = m_armature->getContentSize();
	this->setContentSize(size);

	onBindSprite();
}
Armature* Entity::getArmature(){
	return this->m_armature;
}

void Entity::hurtMe(int iHurtValue) {
	
	if (m_isDead) {
        return;
    }
	if (!this->getArmature()){
		return;
	}
    /* 最小伤害值为1 */
    if (iHurtValue <= getiDefense()) {
        iHurtValue = 1;
    }

    int iCurHP = getiHP();      /* 当前HP */
    int iAfterHP = iCurHP - iHurtValue; /* 被攻击后的HP */

    onHurt(iHurtValue);

    if (iAfterHP > 0) {
        setiHP(iAfterHP);
    }
    else {
        m_isDead = true;
        /* 死亡 */
        onDead();
    
}
}

bool Entity::isDead() {
    return this->m_isDead;
}

void Entity::onDead() {
}

void Entity::onBindSprite() {
}

void Entity::onHurt(int iHurtValue) {
}
