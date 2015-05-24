/*
    �ļ�����    PosBase.h
    �衡����    ����������
    �����ˣ�    ��ľͷ_�ӵ��� (���ͣ�http://www.benmutou.com)
*/
#ifndef _PosBase_H_
#define _PosBase_H_

#include "cocos2d.h"
USING_NS_CC;

class PosBase : public Layer {
public:
    PosBase();
    ~PosBase();

	static PosBase* create(Point pos);
	static PosBase* create(Point pos ,bool isDebug);
	bool init(Point pos);
	bool init(Point pos,  bool isDebug);
	bool initWithTowerType(Point pos, int iHeroType, int iBulletType,bool isDebug);
	
	
    CC_SYNTHESIZE(Point, m_pos, Pos);//getpos,��ȡ���ֵ����������ôû�ж��壿
	CC_SYNTHESIZE(int, heroType, HeroType);
	CC_SYNTHESIZE(int, bulletType, BulletType);

    virtual bool isClickMe(Point pos);   /* �ж������Ƿ���뷶Χ */

    /* ������رյ���ģʽ */
    void setDebug(bool isDebug);    
protected:
    bool m_isDebug;    /* �Ƿ�Ϊ����״̬ */
};

#endif