/*
�������洢bullet��monster��vector��Ϊ����ײ����ʱ�򷽱㴦��
����Ժ��кõ��뷨�ˣ��ٸģ���ʱ����ô�á�
Ϊʲôͷ�ļ��ﲻ���hero�Ϳ��ԣ������bulletbase��monster�Ͳ����ԣ�
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

	//�Ժ���Ƶ�ʱ��Ҫ��get��Ƴ�public�ģ�����set��Ƴ�private�ģ�setʱ��ʹ��notify�������������ٸ��ģ������и��������������˵
	//���˳�ֵ�������⣬�����ط��ͳ�ֵ����йصĶ��Ǽ��٣�
	//player�ĸ������ԣ������ȽϺã���Ȼ����setmoney������ȴ���ı��ļ���ĸ���������Ŀ��ֻ��notify���Ը��ġ�
	CC_SYNTHESIZE(Value, money, Money);
	CC_SYNTHESIZE(Value, level, Level);
	CC_SYNTHESIZE(Value, throwTollgate, throwTollgate);

	CC_SYNTHESIZE(Value, id, Id);

	//���ĸ���player����
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

	int m_index;//���ǹؿ���

	void clear();
private:
	static GlobalClient* m_GlobalClient;
	ValueMap m_value;


    CC_SYNTHESIZE(int, m_iCurTollgateLevel, iCurTollgateLevel);
};

#endif