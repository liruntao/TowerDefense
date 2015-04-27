/*

	创建一个子弹容器（10个，bulletbase类型的），声明一个子弹容器，获得一个未被使用的子弹，getanyunusedbullet（），
	创建一个逻辑判断子弹是否击中目标，击中的话，根据传进来的攻击力，减少目标的血量。

	子弹是绑定在子弹管理器里的，即，bulletnormal绑定在manager上，manager绑定在layer上。

	创建子弹的时候，把它push_back进bulletvector，等移除的时候，再把它从bulletvector中eraser，

*/
#ifndef _BulletManager_H_
#define _BulletManager_H_

#include "cocos2d.h"
USING_NS_CC;

#define BULLET_MAX_CACHE_NUM 10   // 子弹缓存最大数量

class BulletBase;
class BulletManager : public Node {
public:
    BulletManager();
    ~BulletManager();
    static BulletManager* create(int iCur);
    bool init(int iCur);
	BulletBase* createBullets(int iCur);

private:
	BulletBase* m_bullet;

};

#endif