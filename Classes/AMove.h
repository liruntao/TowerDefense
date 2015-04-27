#ifndef __AMove_H__
#define __AMove_H__


//������ŵ���һ��ShortestPathStep��AMove���˽����
// ����������ǲ�̫���ף�˽���ࡣ
//��Ȼ�����е�˵�ýṹ�壬���Ҹо�struct��class���𲻴�ɡ���
//�Ժ�����Ҫ��ʱ����˵

#include "cocos2d.h"
#include "TollgateMapLayer.h"

USING_NS_CC;

class AMove :public Node{
public:
	AMove();
	~AMove();

	static AMove* createWithLayer(TollgateMapLayer* layer);
	bool initWithLayer(TollgateMapLayer* layer);

	void moveToward(const Point& fromPos,const Point& toPos);
	
private:
	TollgateMapLayer* _layer;


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

	void insertInOpenSteps(ShortestPathStep *step);
	int computeHScoreFromCoordToCoord(const Point &fromCoord, const Point&toCoord);
	int costToMoveFromStepToAdjacentStep(const ShortestPathStep* fromStep, const ShortestPathStep* toStep);

	ssize_t getStepIndex(const Vector<ShortestPathStep*>&steps, const ShortestPathStep* step);
	//��������������ʱû��
	void constructPathAndStartAnimationFromStep(ShortestPathStep* step);
	void popStepAndAnimate();

};



#endif