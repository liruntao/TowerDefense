#ifndef __GLOBAL_DEFINE_H__
#define __GLOBAL_DEFINE_H__
/*
û��
*/

#include "cocos2d.h"

/* ����bool����������get��set���� */
#define CC_SYNTHESIZE_BOOL(varName, funName)\
protected: bool varName; \
public: bool is##funName(void) const { return varName; }\
public: void set##funName(bool var){ varName = var; }


/* ��Ϣ�ɷ� */
#define NOTIFY cocos2d::NotificationCenter::getInstance()
static const int DRAG_BODYS_TAG = 0x80;

//��ײ���ʱ�����������tag
static const int BULLET_ONETAG = 100;
static const int BULLET_SEVERALTAG = 101;
static const int MONSTERTAG = 102;
static const int MONSTERBODYTAG = 103;

//��ײ���bitֵ
//һ��body��CategoryBitmask����һ��body��ContactTestBitmask���߼���Ľ��������0ʱ���Ӵ��¼��������������򲻷��͡�
//һ��body��CategoryBitmask����һ��body��CollisionBitmask���߼�����������0ʱ������ײ��������ײ
//setCategoryBitmask(0x08);    // 1000
//setContactTestBitmask(0x08); // 0010
//setCollisionBitmask(0x01);// 0001


//���hero��monster��monsterBullet�ᷢ����ײ��������ײ��⣬hero��hero�ᷢ����ײ��hero�ͳ�ǽ�ᷢ����ײ,hero��herobullet�Ȳ�������ײ��Ҳ�������ص�����
//hero
static const int HERO_CATEGORY_BITMASK = 0x08;//1000
static const int HERO_CONTACTTEST_BITMASK = 0x01;//0001
static const int HERO_COLLISION_BITMASK = 0x01;//0001

//monster
static const int MONSTER_CATEGORY_BITMASK = 0x08;//1000
static const int MONSTER_CONTACTTEST_BITMASK = 0x08;//1000
static const int MONSTER_COLLISION_BITMASK = 0x08;//0001

//heroBullet
static const int HEROBULLET_CATEGORY_BITMASK = 0x08;//1000
static const int HEROBULLET_CONTACTTEST_BITMASK = 0x08;//0010
static const int HEROBULLET_COLLISION_BITMASK = 0x01;//0001

//monsterBullet
static const int MONSTERBULLET_CATEGORY_BITMASK = 0x08;//1000
static const int MONSTERBULLET_CONTACTTEST_BITMASK = 0x08;//0010
static const int MONSTERBULLET_COLLISION_BITMASK = 0x01;//0001

//monsterBody
static const int MONSTERBODY_CATEGORY_BITMASK = 0x08;//1000
static const int MONSTERBODY_CONTACTTEST_BITMASK = 0x08;//1000
static const int MONSTERBODY_COLLISION_BITMASK = 0x01;//0001
#endif