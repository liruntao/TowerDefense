#include "TowerPos.h"

TowerPos::TowerPos() {
    m_pos = Point(0, 0);
    m_isDebug = false;

}

TowerPos::~TowerPos() {
}

TowerPos* TowerPos::create(Point pos) {
    TowerPos* tPos = new TowerPos();

    if(tPos && tPos->init(pos)) {
		
		tPos->autorelease();
    }
    else {
        CC_SAFE_DELETE(tPos);
    }

    return tPos;
}

TowerPos* TowerPos::create( Point pos,int herotype,int bullettype, bool isDebug ) {
    TowerPos* tPos = new TowerPos();

    if(tPos && tPos->init(pos, herotype,bullettype,isDebug)) {
        tPos->autorelease();
    }
    else {
        CC_SAFE_DELETE(tPos);
    }

    return tPos;
}
bool TowerPos::init(Point pos, int herotype, int bullettype,bool isDebug) {
    bool bRet = false;

    do {
	

		CC_BREAK_IF(!PosBase::initWithTowerType(pos, herotype, bullettype, isDebug));

        bRet = true;
    } while (0);

    return bRet;
}

bool TowerPos::init(Point pos) {
    bool bRet = false;

    do {
		CC_BREAK_IF(!PosBase::init(pos));
        bRet = true;
    } while (0);

    return bRet;
}


/*��Ҫ��������������

void TowerPos::draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) {
    if(m_isDebug) {
        _customCommand.init(_globalZOrder);
        _customCommand.func = CC_CALLBACK_0(TowerPos::onDraw, this, transform, transformUpdated);
        renderer->addCommand(&_customCommand);
    }
}

void TowerPos::onDraw(const kmMat4 &transform, bool transformUpdated)
{
    // ���Ʋ��Է��� 

    kmGLPushMatrix();
    kmGLLoadMatrix(&transform);

    glLineWidth(5.0f);	// ���û��ʴ�ϸ

    // ���ƾ��� 
    Point srcPos = Point(m_pos.x - RADIUS, m_pos.y + RADIUS);
    Point destPos = Point(m_pos.x + RADIUS, m_pos.y - RADIUS);
    DrawPrimitives::drawRect(srcPos, destPos);

    glLineWidth(1);	// �ָ����ʴ�ϸ

    kmGLPopMatrix();// ��������
}*/

bool TowerPos::isClickMe( Point pos ) {
    Point srcPos = Point(m_pos.x - RADIUS, m_pos.y + RADIUS);
    Point destPos = Point(m_pos.x + RADIUS, m_pos.y - RADIUS);

    if(pos.x >= srcPos.x && pos.x <= destPos.x && pos.y <= srcPos.y && pos.y >= destPos.y) {
        return true;
    }

    return false;
}

