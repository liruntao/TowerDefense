/*
    �ļ�����    BulletBase.h
    �衡����    �ӵ�����
    �����ˣ�    ��ľͷ_�ӵ��� (���ͣ�http://www.benmutou.com)

	�趨�ӵ��Ĺ������͹����ٶȣ��ж��Ƿ�����Ŀ�꣬����������ӵ���Ŀ��m_aim=������Ŀ�꣬
	�����Ի�ȡ���ӵ���Ŀ��getaim����������һ�������ж��ӵ��Ƿ񵽴�Ŀ�ĵأ����������ӵ��Ƿ�����ʹ���У���ͬһʱ��ֻ�ɱ�ʹ��һ�Σ�
	���Ի��ʹ��״̬���趨�Ƿ�ʹ�á�

*/
#ifndef _BulletBase_H_
#define _BulletBase_H_

#include "Entity.h"

/* �ӵ��ٶ� */
#define SPEED_DEFAULT 10
#define SPEED_NORMAL 5

class BulletBase : public Entity {
public:
    BulletBase();
    ~BulletBase();

    /* ��������Ŀ�� */
    void lockAim(Entity* entity);

    /* ��ȡ����Ŀ�� */
    Entity* getAim();

	/* �Ƿ�����ʹ�� */
    void setUsed(bool isUsed);
    bool isUsed();

	/* �Ƿ񵽴�Ŀ�� */
    bool isArrive();
	/* ��������Ŀ��ʱ���ã��������������� */
	virtual void onLockAim(Entity* aim) = 0;
protected:
  
	Entity* m_aim;	    /* ����Ŀ�� */

    bool m_isArrive;    /* �Ƿ�ﵽ�˹���Ŀ�꣨�Ƿ񹥻���Ŀ�꣩ */
private:
    bool m_isUsed;      /* ����Ƿ��Ѿ���ʹ���� */

    CC_SYNTHESIZE(int, m_iAtkValue, iAtkValue); /* ������ */
    CC_SYNTHESIZE(int, m_iSpeed, iSpeed);       /* �ٶ� */
	CC_SYNTHESIZE(float, rate, Rate);//�������
	CC_SYNTHESIZE(int, type, Type);//�ӵ�����

};

#endif