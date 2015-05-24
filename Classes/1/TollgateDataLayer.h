/*
    文件名：    TollgateDataLayer.h
    描　述：    关卡数据图层
    创建人：    笨木头_钟迪龙 (博客：http://www.benmutou.com)
*/
#ifndef _TollgateDataLayer_H_
#define _TollgateDataLayer_H_


#include "cocos2d.h"
USING_NS_CC;

#include "editor-support/cocostudio/CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC_EXT;

class TollgateDataLayer : public Layer {
public:
    TollgateDataLayer();
    ~TollgateDataLayer();

    CREATE_FUNC(TollgateDataLayer);
    virtual bool init();

    int getiTowerSoulNum(); /* 获取当前塔魂数量 */
    int getiMagicNum();     /* 获取当前魔力数量 */
private:
    void recvRefreshTowerSoulNum(Ref* pData);
    void recvRefreshMonsterNum(Ref* pData);
    void recvRefreshMagicNum(Ref* pData);
    void recvAllMonsterDead(Ref* pData);

    int m_iTowerSoulNum;    /* 塔魂数量 */
    int m_iMonsterNum;      /* 怪物数量 */
    int m_iMagicNum;        /* 魔力数量 */

    Text* m_towerSoulLab;   /* 塔魂标签 */
    Text* m_monsterLab;     /* 怪物标签 */
    Text* m_magicLab;      /* 魔力标签 */
};

#endif