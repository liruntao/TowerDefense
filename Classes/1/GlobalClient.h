/*
单例，存储bullet和monster的vector，为了碰撞检测的时候方便处理。
这个以后有好的想法了，再改，暂时先这么用。
为什么头文件里不添加hero就可以，不添加bulletbase和monster就不可以？
*/
#ifndef GlobalClient_H
#define GlobalClient_H

#include <iostream>
#include "cocos2d.h"
//#include "BulletManager.h"
//#include "MonsterManager.h"
//#include "HeroManager.h"
#include "BulletBase.h"
#include "Monster.h"
#include "Hero.h"


#include "cocos2d.h"
USING_NS_CC;

class GlobalClient : public Ref 
{
public:
	static GlobalClient* getInstance();
	CREATE_FUNC(GlobalClient);
	~GlobalClient();
	virtual bool init();

	Vector<BulletBase*> BulletVector;
	Vector<Monster*> MonsterVector;
	Vector<Hero*> HeroVector;

	void initPlayer();

	//以后设计的时候要将get设计成public的，而将set设计成private的，set时，使用notify传过来，这里再更改（可以有个检测条件，比如说
	//除了充值界面以外，其他地方和充值金币有关的都是减少）
	//player的各个属性，这样比较好，虽然可以setmoney，但是却不改变文件里的各种属性数目，只有notify可以更改。
	CC_SYNTHESIZE(Value, money, Money);
	CC_SYNTHESIZE(Value, level, Level);
	CC_SYNTHESIZE(Value, throwTollgate, throwTollgate);

	CC_SYNTHESIZE(Value, id, Id);

	//更改各种player属性
	void recvRefreshPlayerMoney(Ref* pData);
	void recvRefreshPlayerIndex(Ref* pData);
	void recvRefreshPlayerStar(Ref* pData);
	void recvRefreshPlayerThrowTollgate(Ref* pData);

	void changeStar(int index, int Star);
	
	Value getValue(std::string key);
	void changeValue(std::string key, Value value);
	void setValue(std::string key, Value value);
	ValueVector getValueVector(std::string key);


	int getStar(int curNum);

	int m_index;//星星关卡数

	void clear();
private:
	static GlobalClient* m_GlobalClient;
	ValueMap m_value;


    CC_SYNTHESIZE(int, m_iCurTollgateLevel, iCurTollgateLevel);
};

#endif