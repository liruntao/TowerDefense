#include "PosLoadUtil.h"
#include "EnumPosType.h"
#include "TowerPos.h"
#include "MonsterPos.h"

PosLoadUtil* PosLoadUtil::m_posLoadUtil = NULL;

PosLoadUtil* PosLoadUtil::getInstance() {
    if (m_posLoadUtil == NULL) {
        m_posLoadUtil = new PosLoadUtil();

        if (m_posLoadUtil && m_posLoadUtil->init()) {
            m_posLoadUtil->autorelease();
            m_posLoadUtil->retain();
        }
        else {
            CC_SAFE_DELETE(m_posLoadUtil);
        }
    }

    return m_posLoadUtil;
}

bool PosLoadUtil::init() {

    return true;
}
Vector<PosBase*> PosLoadUtil::loadPosWithFile(const char* sFilePath, EnumPosType enPosType, Node* container, int iLevel, bool isDebug) {
    Vector<PosBase*> posList;

    ValueMap fileDataMap = FileUtils::getInstance()->getValueMapFromFile(sFilePath);

   
	//����Valuemap
	for (auto it = fileDataMap.begin(); it != fileDataMap.end();++it){
        Value value = it->second;
        ValueMap data = value.asValueMap();

        /* ����������� */
        PosBase*  posBase = NULL;
        switch (enPosType) {
        case enTowerPos:
			posBase = TowerPos::create(Point(data["x"].asInt(), data["y"].asInt()), data["herotype"].asInt(), data["bullettype"].asInt(), isDebug);
            break;
        case enMonsterPos:
            posBase = MonsterPos::create(Point(data["x"].asInt(), data["y"].asInt()), isDebug);
            break;
        default:
			posBase = TowerPos::create(Point(data["x"].asInt(), data["y"].asInt()), data["herotype"].asInt(), data["bullettype"].asInt(), isDebug);
            break;
        }
        posList.pushBack(posBase);

        if (container != NULL) {
            container->addChild(posBase, iLevel);
        }
    }
    
    return posList;
}

