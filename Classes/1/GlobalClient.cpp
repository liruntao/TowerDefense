#include "GlobalClient.h"
#include "GlobalDefine.h"
#include <iostream>
#include <string.h>
#include <string>
//star��ÿ�ص�������Ŀ
//throwTollgate��ͨ�صĹؿ���
using namespace std;
GlobalClient* GlobalClient::m_GlobalClient = NULL;

GlobalClient::~GlobalClient(){
	NOTIFY->removeAllObservers(this);
}

GlobalClient* GlobalClient::getInstance(){
    if (m_GlobalClient == NULL) {
        m_GlobalClient = new GlobalClient();
        if (m_GlobalClient && m_GlobalClient->init()) {
            m_GlobalClient->autorelease();
            m_GlobalClient->retain();

        }
        else {
            CC_SAFE_DELETE(m_GlobalClient);
            m_GlobalClient = NULL;
        }
    }

    return m_GlobalClient;
}

void GlobalClient::clear(){
	BulletVector.clear();
	HeroVector.clear();
	MonsterVector.clear();
}



bool GlobalClient::init(){
	
	//��ʼ��
	//initPlayer();
	

	
	NOTIFY->addObserver(this,
		callfuncO_selector(GlobalClient::recvRefreshPlayerMoney),
		"PlayerMoneyChange",
		NULL);
	//���ݹ���index��star
	NOTIFY->addObserver(this,
		callfuncO_selector(GlobalClient::recvRefreshPlayerIndex),
		"PlayerIndexChange",
		NULL);
	NOTIFY->addObserver(this,
		callfuncO_selector(GlobalClient::recvRefreshPlayerStar),
		"PlayerStarChange",
		NULL);
	NOTIFY->addObserver(this,
		callfuncO_selector(GlobalClient::recvRefreshPlayerThrowTollgate),
		"PlayerThrowTollgateChange",
		NULL);


	return true;
}

void GlobalClient::initPlayer(){
	//�ؿ�
	//String lo;
	//const char* i1= "throwTollgate";
	//std::string sFilePath = StringUtils::toString(i1);
	string thorowTollgateKey = "throwTollgate";
	int tTollgate = 1;
	Value TTollgate = Value(tTollgate);
	changeValue(thorowTollgateKey, TTollgate);
	//ÿ�ص�����
	//const char* i2="star";
	//std::string sFilePath1 = StringUtils::toString(i2);
	string starKey = "star";
	int i = 1;
	int i1 = 1;
	ValueMap initStar;
	initStar[StringUtils::toString(i)] = Value(i1);
	Value starFileData = Value(initStar);
	changeValue(starKey, starFileData);
	//�༭�ؿ����ѡ��Ӣ�۵�����
	string heroTypeKey = "herotype";
	ValueVector initHeroType = { Value(1), Value(2), Value(3) };
	Value heroFileData = Value(initHeroType);
	changeValue(heroTypeKey, heroFileData);
	//�༭�ؿ����ѡ���ӵ�������
	string bulletTypeKey = "bullettype";
	ValueVector initBulletType = { Value(1), Value(2), Value(3) };
	Value bulletFileData = Value(initBulletType);
	changeValue(bulletTypeKey, bulletFileData);
}
void GlobalClient::recvRefreshPlayerMoney(Ref* pData){
	int m_money = (int)pData;
	if (m_money < 0){
		int curMoney = money.asInt();
		curMoney += m_money;
		money = (Value)curMoney;
	}
}
void GlobalClient::recvRefreshPlayerThrowTollgate(Ref* pData){
	int curThrowTollgate = (int)pData;
	if (curThrowTollgate > 0){
		string thorowTollgateKey = "throwTollgate";
	
		Value curValue = Value(curThrowTollgate);
		changeValue(thorowTollgateKey, curValue);
	}
}
void GlobalClient::recvRefreshPlayerIndex(Ref* pData){
	m_index = (int)pData;
}
void GlobalClient::recvRefreshPlayerStar(Ref* pData){
	int curStar = (int)pData;
	if (curStar > 0 && curStar < 4){
		changeStar(m_index, curStar);
	}
}
//���Կ�����ǰ�����Id��������
void GlobalClient::changeStar(int index, int changeStar) {
	string starKey = "star";
	Value value = getValue(starKey);
	ValueMap originalValue = value.asValueMap();
	originalValue[StringUtils::toString(index)] = Value(changeStar);
	Value fileData = Value(originalValue);
	changeValue(starKey, fileData);

}

Value GlobalClient::getValue(std::string key){
	
	string sFilePath = StringUtils::format("Player.plist");
	m_value = FileUtils::getInstance()->getValueMapFromFile(sFilePath.c_str());
	Value curValue = m_value[StringUtils::toString(key)];
	return curValue;
}

void GlobalClient::changeValue(std::string key, Value value){
	//string sFilePath = StringUtils::format("Player.plist");
	string sFilePath = "Player.plist";
	ValueMap fileDataMap = FileUtils::getInstance()->getValueMapFromFile(sFilePath.c_str());
	fileDataMap[key] = Value(value);
	//FileUtils::getInstance()->writeToFile(fileDataMap, sFilePath.c_str());
	FileUtils::getInstance()->writeToFile(fileDataMap, sFilePath.c_str());
}
//�Ժ�ĳ�getValueMap��
int GlobalClient::getStar(int curNum){
	string starKey="star";
	Value curValue = getValue(starKey);
	ValueMap fileData = curValue.asValueMap();
	int originalStar = fileData[StringUtils::toString(curNum)].asInt();
	return originalStar;
}

ValueVector GlobalClient::getValueVector(std::string key){
	string valueVectorKey = key;
	Value curValue = getValue(valueVectorKey);
	ValueVector fileData = curValue.asValueVector();
	
	return fileData;
}