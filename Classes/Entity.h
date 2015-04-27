/*
ʵ����࣬�̳���node��������bindsprite��Ҳ����bind�����Ķ�����
�����������Ȱ�Ӣ�ۡ�monsterʲô��ֱ�Ӽ̳���spriteҪ�ã�Ҫ�������԰󶨸���Ķ�����

����������Ƿ���Ҫ��������������Ϊvirtual����
*/

#ifndef _Entity_H_
#define _Entity_H_

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"
using namespace cocostudio;
USING_NS_CC;

class Entity : public Node {
public:
    Entity();
    ~Entity();

    void bindSprite(Sprite* sprite);
    Sprite* getSprite();

	void bindArmature(Armature *armature);
	Armature* getArmature();
	Armature* m_armature;

    void hurtMe(int iHurtValue);        /* ������ */
    bool isDead();                      /* �Ƿ����� */


protected:
    virtual void onDead();		        /* ʵ������ʱ���� */
    virtual void onBindSprite();        /* �󶨾������ʱ���� */
    virtual void onHurt(int iHurtValue);/* ���˺�ʱ���� */
private:
    Sprite* m_sprite;

    CC_SYNTHESIZE(int, m_ID, ID);	            //ʵ��ID
    CC_SYNTHESIZE(int, m_iModelID, iModelID);	//ģ��ID����ԴID��
    CC_SYNTHESIZE(std::string, m_sName, sName);	//����
    CC_SYNTHESIZE(int, m_iHP, iHP);			    //HP
    CC_SYNTHESIZE(int, m_iDefense, iDefense);	//����
    CC_SYNTHESIZE(int, m_iSpeed, iSpeed);		//�ƶ��ٶ�
    CC_SYNTHESIZE(int, m_iLevel, iLevel);		//�ȼ�
	CC_SYNTHESIZE(int, m_iBaseAtk, iBaseAtk);				//������������û����
	CC_SYNTHESIZE(int, m_iAtkSpeed, iAtkSpeed);				//�����������λ�����룩
	CC_SYNTHESIZE(int, m_iAtkRange, iAtkRange);				//������Χ���뾶��
	CC_SYNTHESIZE(int, m_iUpgradeCostBase, iUpgradeCostBase);	//�������Ļ���ֵ
	CC_SYNTHESIZE(float, m_fUpgradeAtkBase, fUpgradeAtkBase);	//���������ӳ�ϵ��
	CC_SYNTHESIZE(int, cgHp, changeHp);   
	CC_SYNTHESIZE(int, cgDefense, changeDefense);
	CC_SYNTHESIZE(int, cgSpeed, changeSpeed);
	CC_SYNTHESIZE(int, bodycgSpeed, changebodySpeed);
//����Ϊ�������
	CC_SYNTHESIZE(int, m_MetalAtk, MetalAtk);
	CC_SYNTHESIZE(int, m_MetalDefense, MetalDefense);
	CC_SYNTHESIZE(int, m_WoodAtk, WoodAtk);
	CC_SYNTHESIZE(int, m_WoodDefense, WoodDefense);
	CC_SYNTHESIZE(int, m_WaterAtk, WaterAtk);
	CC_SYNTHESIZE(int, m_WaterDefense, WaterDefense);
	CC_SYNTHESIZE(int, m_FireAtk, FireAtk);
	CC_SYNTHESIZE(int, m_FireDefense, FireDefense);
	CC_SYNTHESIZE(int, m_EarthAtk, EarthAtk);
	CC_SYNTHESIZE(int, m_EarthDefense, EarthDefense);


    bool m_isDead;      //����Ƿ�����
};

#endif