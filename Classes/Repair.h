#ifndef _Repair_H_
#define _Repair_H_

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class Repair :public Layer
{
public:
	virtual bool init(int totalBlood) ;
	static Repair* create(Point pos,int totalBlood);
	virtual void updateProgressBar(float dt);


	CC_SYNTHESIZE(bool, timeOut, TimeOut);
	CC_SYNTHESIZE(Point, repairPos, RepairPos);

private:
	ProgressTimer* progressBar;
	Label* nameLabel;
	Label* timeLabel;

	int counter;
	int repairTime;
	float percent;

};



#endif
