/*
整合a*与ControllerSimpleMove的a星movecontrol
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




//把以下3个的Vector<PosBase*> 都换成了Vector<Point>,不对，这三个不能换，是传进来的。
	void checkMoveUpdate(float delta);

	/* 按照给定的坐标点移动 */
	void moveByPosList(Vector<PosBase*>  posList);

	/* 按照给定的坐标点和移动速度 */
	void moveByPosList(Vector<PosBase*> posList, int iSpeed);

	/* 按照给定的坐标点、速度、移动间隔时间移动 */
	void moveByPosList(Vector<PosBase*> posList, int iSpeed, int iSpanTime);

	/* 按照给定坐标移动 */
	void moveByPos(PosBase* pos);
private:
	/* 移动一步 */
	void moveOneStep();
	

	/* 设置下一个移动目的点 */
	void nextMovePos();
private:
	Vector<PosBase*> m_movePosList; // 移动目的列表
	// 当前移动目的地，这个自然也换掉，
	PosBase* m_curDestPos;          

	float m_MoveSpan;       // 移动间隔时间
	float m_fMoveTimeCount; // 计时器

	//以上除create和init以外，都是原始的，以下为新添加的
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
	//以下两个可能暂时没用
	void constructPathAndStartAnimationFromStep(ShortestPathStep* step);
	void popStepAndAnimate();

};



#endif