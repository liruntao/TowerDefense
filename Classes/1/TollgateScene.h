/*
    �ļ�����    TollgateScene.h
    �衡����    �ؿ�����
    �����ˣ�    ��ľͷ_�ӵ��� (���ͣ�http://www.benmutou.com)
*/
#ifndef _TollgateScene_H_
#define _TollgateScene_H_

#include "cocos2d.h"
#include "extensions\cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define TAG_MAP_LAYER 1	    // �ؿ���ͼͼ��TAG
#define TAG_DATA_LAYER 2	// �ؿ�����ͼ��TAG

class TollgateScene : public Layer {
public:
    static Scene* createScene();
    virtual bool init();
	void setPhysicsWorld(PhysicsWorld* world){ m_world = world; };
	PhysicsWorld* getPhysicsWorld() { return m_world; }

    CREATE_FUNC(TollgateScene);
private:
      PhysicsWorld* m_world;


};

#endif