#ifndef _TollgateLevelSelectPage_H_
#define _TollgateLevelSelectPage_H_
#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class TollgateLevelSelectPage :public Node{
public:

	bool initLevelPage(const std::string& bgName, int level);
	static TollgateLevelSelectPage* create(const std::string& bgName, int level);
	void menuStartCallback(Ref* pSender);

};



#endif