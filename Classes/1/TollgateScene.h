/*
    文件名：    TollgateScene.h
    描　述：    关卡场景
    创建人：    笨木头_钟迪龙 (博客：http://www.benmutou.com)
*/
#ifndef _TollgateScene_H_
#define _TollgateScene_H_

#include "cocos2d.h"
#include "extensions\cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define TAG_MAP_LAYER 1	    // 关卡地图图层TAG
#define TAG_DATA_LAYER 2	// 关卡数据图层TAG

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