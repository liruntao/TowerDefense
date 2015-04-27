#include "TollgateDataLayer.h"
#include "GlobalDefine.h"
#include "SceneManager.h"
#include "GlobalClient.h"
#include <string>
using namespace std;

TollgateDataLayer::TollgateDataLayer() {
    m_iTowerSoulNum = 0;    /* 塔魂数量 */
    m_iMonsterNum = 0;      /* 怪物数量 */
    m_iMagicNum = 0;        /* 魔力数量 */
}
TollgateDataLayer::~TollgateDataLayer() {
    NOTIFY->removeAllObservers(this);
}
bool TollgateDataLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    /* 加载UI */
    auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("TollgateUI/TollgateUI_1.ExportJson");
    this->addChild(UI);

    UI->setTouchEnabled(false);

    /* 塔魂标签 */
    m_towerSoulLab = (Text*)Helper::seekWidgetByName(UI, "towerSoulLab");

    /* 怪物标签 */
    m_monsterLab = (Text*)Helper::seekWidgetByName(UI, "monsterNumLab");

    /* 魔力标签 */
    m_magicLab = (Text*)Helper::seekWidgetByName(UI, "magicLab");

    /* 订阅消息 */
    NOTIFY->addObserver(this,
        callfuncO_selector(TollgateDataLayer::recvRefreshTowerSoulNum),
        "TowerSoulChange",
        NULL);

    NOTIFY->addObserver(this,
        callfuncO_selector(TollgateDataLayer::recvRefreshMonsterNum),
        "MonsterNumChange",
        NULL);

    NOTIFY->addObserver(this,
        callfuncO_selector(TollgateDataLayer::recvRefreshMagicNum),
        "MagicChange",
        NULL);
    NOTIFY->addObserver(this,
        callfuncO_selector(TollgateDataLayer::recvAllMonsterDead),
        "AllMonsterDead",
        NULL);
    return true;
}

void TollgateDataLayer::recvRefreshTowerSoulNum(Ref* pData){
    int iAltValue = (int)pData;
    m_iTowerSoulNum += iAltValue;
    m_towerSoulLab->setText(StringUtils::toString(m_iTowerSoulNum));
}

void TollgateDataLayer::recvRefreshMonsterNum(Ref* pData){
    int iAltValue = (int)pData;
    m_iMonsterNum += iAltValue;
    m_monsterLab->setText(StringUtils::toString(m_iMonsterNum));
}

void TollgateDataLayer::recvRefreshMagicNum(Ref* pData){
    int iAltValue = (int)pData;
    m_iMagicNum += iAltValue;
    m_magicLab->setText(StringUtils::toString(m_iMagicNum));

    /* 魔力值小于等于0，游戏失败 */
    if (m_iMagicNum <= 0) {
        SceneManager::getInstance()->changeScene(SceneManager::en_GameOverScene);
    }
}

//在这里设置星星，和过关
void TollgateDataLayer::recvAllMonsterDead(Ref* pData) {
    if (m_iMagicNum > 0) {
		auto star = 0;
		if (m_iMagicNum <= 30){ star = 1; }
		else if (m_iMagicNum <= 60){ star = 2; }
		else if (m_iMagicNum >60){ star = 3; }

		int curNum = GlobalClient::getInstance()->getiCurTollgateLevel();
		auto levelnum =curNum  + 1;
		string throwTollgateKey="throwTollgate";

		//if (levelnum > UserDefault::getInstance()->getIntegerForKey("levelNum"))
		if(levelnum>GlobalClient::getInstance()->getValue(throwTollgateKey).asInt()){
			NOTIFY->postNotification("PlayerThrowTollgateChange", (Ref*)levelnum);

		}
		//加星星
		if (star > GlobalClient::getInstance()->getStar(curNum));
		{
			NOTIFY->postNotification("PlayerIndexChange", (Ref*)curNum);
			NOTIFY->postNotification("PlayerStarChange", (Ref*)star);

		}
		

		SceneManager::getInstance()->changeScene(SceneManager::en_WinScene);

    }
}

int TollgateDataLayer::getiTowerSoulNum() {
    return m_iTowerSoulNum;
}

int TollgateDataLayer::getiMagicNum(){
    return m_iMagicNum;
}
