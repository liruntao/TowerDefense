/*
����a*��ControllerSimpleMove��a��movecontrol
*/
#ifndef _ControllerAMove_H_
#define _ControllerAMove_H_

#include "ControllerMoveBase.h"
#include "Entity.h"
#include "PosBase.h"
#include "TollgateMapLayer.h"

class PosBase;
class TollgateMapLayer;
class ControllerAMove : public ControllerMoveBase {
public:
	ControllerAMove();
	~ControllerAMove();

	cocos2d::Animation *cur_Animation;
	cocos2d::Action *cur_Animate;
	Armature* cur_armature ;


	cocos2d::Animation *_facingForwardAnimation;
	cocos2d::Animation *_facingBackAnimation;
	cocos2d::Animation *_facingLeftAnimation;
	cocos2d::Animation *_facingRightAnimation;
	
	cocos2d::Animation *createAnimation(const char *animType);

	static ControllerAMove* createWithLayer(Entity* entity, TollgateMapLayer* layer);

	bool initWithLayer(Entity* entity, TollgateMapLayer* layer);

	void moveToward(const Point& fromPos, const Point& toPos);




//������3����Vector<PosBase*> ��������Vector<Point>,���ԣ����������ܻ����Ǵ������ġ�
	void checkMoveUpdate(float delta);

	/* ���ո�����������ƶ� */
	void moveByPosList(Vector<PosBase*>  posList);

	/* ���ո������������ƶ��ٶ� */
	void moveByPosList(Vector<PosBase*> posList, int iSpeed);

	/* ���ո���������㡢�ٶȡ��ƶ����ʱ���ƶ� */
	void moveByPosList(Vector<PosBase*> posList, int iSpeed, int iSpanTime);

	/* ���ո��������ƶ� */
	void moveByPos(PosBase* pos);
private:
	/* �ƶ�һ�� */
	void moveOneStep();
	

	/* ������һ���ƶ�Ŀ�ĵ� */
	void nextMovePos();
private:
	Vector<PosBase*> m_movePosList; // �ƶ�Ŀ���б�
	// ��ǰ�ƶ�Ŀ�ĵأ������ȻҲ������
	PosBase* m_curDestPos;          

	float m_MoveSpan;       // �ƶ����ʱ��
	float m_fMoveTimeCount; // ��ʱ��

	//���ϳ�create��init���⣬����ԭʼ�ģ�����Ϊ����ӵ�
private:
	TollgateMapLayer* _layer;
	TMXLayer* _meta;

private:
	class ShortestPathStep :public Node{
	public:
		ShortestPathStep();
		~ShortestPathStep();

		static ShortestPathStep* createWithPosition(const Point &pos);
		bool initWithPosition(const Point &pos);

		int getFScore() const;
		bool isEqual(const ShortestPathStep* other) const;
		std::string getDescription() const;

		CC_SYNTHESIZE(Point, _tileposition, tilePosition);
		CC_SYNTHESIZE(int, _gScore, GScore);
		CC_SYNTHESIZE(int, _hScore, HScore);
		CC_SYNTHESIZE(ShortestPathStep*, _parent, Parent);
	};

	Vector<ShortestPathStep*> _spOpenSteps;
	Vector<ShortestPathStep*> _spClosedSteps;
	Vector<ShortestPathStep*> _shortestPath;
	Vector<PosBase*> tilePositionshortestPath;

	void insertInOpenSteps(ShortestPathStep *step);
	int computeHScoreFromCoordToCoord(const Point &fromCoord, const Point&toCoord);
	int costToMoveFromStepToAdjacentStep(const ShortestPathStep* fromStep, const ShortestPathStep* toStep);

	ssize_t getStepIndex(const Vector<ShortestPathStep*>&steps, const ShortestPathStep* step);
	//��������������ʱû��
	void constructPathAndStartAnimationFromStep(ShortestPathStep* step);
	void popStepAndAnimate();

};



#endif