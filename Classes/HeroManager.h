/*
    �ļ�����    HeroManager.h
    �衡����    Ӣ�۹�����
    �����ˣ�    ��ľͷ_�ӵ��� (���ͣ�http://www.benmutou.com)

    �������ڣ�   2013.05.04
*/
#ifndef _HeroManager_H_
#define _HeroManager_H_

#include "cocos2d.h"
USING_NS_CC;
#include "HeroPanleLayer.h"
#include "TowerBorder.h"
#define TOWER_POS_LAYER_LVL 5       // ������Ĳ��
#define TOWER_BORDER_LAYER_LVL 8    // ��̨�Ĳ��
#define TOWER_LAYER_LVL 10          // ���Ĳ��


class PosBase;
class Monster;
class HeroManager : public Node {
public:
    HeroManager();
    ~HeroManager();
    static HeroManager* createWithLevel(int iCurLevel);

    bool initWithLevel(int iCurLevel);

    void logic(float dt, Vector<Monster*> monsterList);

	//����ӵģ�Ϊ�˼���ѡ��������layer��
	// bool onTouchBegan(Touch* touch, Event* event);// override,ʹ��override�����ĳ�Ա����������д�����Ա
//	 bool onTouchEnded(Touch* touch, Event* event) ;//override,����д�Ǿ�û��Ҫ��virtual��

	//virtual void update(float dt) override;

	//�µļ���Tmxmap��create,�����ã�����c++ûѧ�ã��Ա���ʲô���˽�Ĳ���
//	������һ��position���ԣ�createͨ����Ƴ�staticʱ��Ӧ����Ϊ���ñ������ʵ�ʱ�򷽱㡣
	//û��Ҫ��ֱ�ӵ����ȡposition���ɣ������õ���ȥ������position��
	static HeroManager* createWithLevelAndPosition(int iCurLevel,Point* position);
//����һ��ȫ�ֱ������ԣ�ʹ��create����������
	TowerBorder* clickBorder;

private:
    /* ������������������ */
    Vector<PosBase*> m_towerPosList;

    /* ���������̨���� */
    Vector<TowerBorder*> m_towerBorderList;

    /* ������̨ */
    void createTowerBorder(int iCurLevel);

    /* �������꣬������������� */
    void createTowerPos(Point pos);

    /* �ҵ�����������̨���� */
    TowerBorder* findClickTowerBorder(Point pos);

	///Ϊ��ɾ�������㣬���ǰѴ���Heropanellayerȫ��ɾ����
	TowerBorder* border;

	Point Heroborderposition;

	//��Ϣ��ӣ�Ϊ���ڵ����ʱ�򣬴���hero
	void recvTowerBorderPosition(Ref* pData);//������Ǹ�������λ�á�
	void recvTowerHeroType(Ref* pData);//�������ࡣ
	Point recvTBPosition;//��������


	//����ӵģ�Ϊ�˼���ѡ��������layer���öຯ��Ӧ�ö�û��
	Vector<Node*>pointsVector;
	float offX;
	bool isTouchEnable;
	HeroPanleLayer* chooseTowerpanel;
	void addTowerChoosePanel(Point position);
	Point covertTotileCoord(Point position);
	Point covertToMatrixCoord(Point position);
	void checkAndAddTowerPanle(Point position);
	void CollisionDetection();

	Point towerPos;
	void initPointsVector(float offX);
	void addTower();
	Hero** towerMatrix;

	const TMXTiledMap* map;
	TMXLayer* bgLayer;
	TMXObjectGroup* objects;

};

#endif