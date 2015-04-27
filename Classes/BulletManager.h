/*

	����һ���ӵ�������10����bulletbase���͵ģ�������һ���ӵ����������һ��δ��ʹ�õ��ӵ���getanyunusedbullet������
	����һ���߼��ж��ӵ��Ƿ����Ŀ�꣬���еĻ������ݴ������Ĺ�����������Ŀ���Ѫ����

	�ӵ��ǰ����ӵ���������ģ�����bulletnormal����manager�ϣ�manager����layer�ϡ�

	�����ӵ���ʱ�򣬰���push_back��bulletvector�����Ƴ���ʱ���ٰ�����bulletvector��eraser��

*/
#ifndef _BulletManager_H_
#define _BulletManager_H_

#include "cocos2d.h"
USING_NS_CC;

#define BULLET_MAX_CACHE_NUM 10   // �ӵ������������

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