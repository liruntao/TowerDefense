/*
流程：在其他类里先创建一个controsimplemove，这个ctr绑定到一个实体上，（此时，str的schedule就应该开始调用了，
但因为m_ismoveing为FALSE，因此应该不执行任何操作）该实体取得一个postlist列表后，调用movebypolist函数，此时传入
ispeed，spantime，和最重要到poslist，movebypolist逐级调用，并实例化各种变量，将m_ismoving置为ture，并开启nextmove的
第一步操作（这里尚有一个操作，就是将此类里的poslist置空，防止因为原来移动时，由于终止移动导致list不为空，而出现错误）。
此后，update就开始运行，调用函数按照poslist里的顺序，一步一步moveto到destpos（需要说明的是，nextmove每取一个
坐标，就调用eraser将此坐标移除。moveonestrp有一个判断语句，当poslist的size大于0时，代表尚未到达destpos，才调用moveonestep）


*/
#include "ControllerSimpleMove.h"

ControllerSimpleMove::ControllerSimpleMove() {
    m_fMoveTimeCount = 0;
    m_MoveSpan = 0;
}

ControllerSimpleMove::~ControllerSimpleMove() {
}

ControllerSimpleMove* ControllerSimpleMove::create( Entity* entity ) {
	ControllerSimpleMove* ctrlSimpleMove = new ControllerSimpleMove();

	if(ctrlSimpleMove && ctrlSimpleMove->init(entity)) {
		ctrlSimpleMove->autorelease();
	}
	else {
		CC_SAFE_DELETE(ctrlSimpleMove);
	}
	return ctrlSimpleMove;
}

bool ControllerSimpleMove::init( Entity* entity ) {
    this->m_entity = entity;

    this->schedule(schedule_selector(ControllerSimpleMove::checkMoveUpdate));

	return true;
}

//首先传入这个函数，置m_ismoving为true，才能开始schedule。
//取speed的值，span的值，以及调用nextmovepos获得下一步移动坐标的值
void ControllerSimpleMove::moveByPosList(Vector<PosBase*> posList) {
	if(posList.size() < 1) {
		return;
	}

	this->m_movePosList.clear();
    this->m_movePosList.pushBack(posList);

	nextMovePos();

	this->m_isMoving = true;
}

void ControllerSimpleMove::moveByPosList(Vector<PosBase*> posList, int iSpeed) {
    this->m_iSpeed = iSpeed;
    moveByPosList(posList);
}

void ControllerSimpleMove::moveByPosList(Vector<PosBase*> posList, int iSpeed, int iSpanTime) {
    m_MoveSpan = iSpanTime;
    moveByPosList(posList, iSpeed);
}
void ControllerSimpleMove::checkMoveUpdate( float delta ) {
	if(m_isMoving) {
        m_fMoveTimeCount += delta * 1000;

        if(m_fMoveTimeCount >= m_MoveSpan) {//计时器，每帧加delta*1000，如果大于设定的移动时间间隔，则将计时器置零，然后moveonesetp，应该是移动一步
            m_fMoveTimeCount = 0;
		    moveOneStep();
        }
	}
}

void ControllerSimpleMove::moveOneStep() {
	Sprite* sprite = m_entity->getSprite();//初始化时，将调用simplemove的entity传入，m_entity=entity

	if(sprite != NULL) {
		Point entityPos = m_entity->getPosition();
		Point curDestPos = m_curDestPos->getPos();//这里为什么用getpos，都没有定义？
		
		/* 取得下一个移动坐标点 */
        entityPos = getNextPos(entityPos, curDestPos);

		/* 移动实体 */
		m_entity->setPosition(entityPos);

		/* 到达当前目的地，开始下一个目的地 */
		if(entityPos.x == curDestPos.x && entityPos.y == curDestPos.y) {
			if(m_movePosList.size() > 0) {
				nextMovePos();
			}
            else {
                /* 移动结束 */
                if (m_moveEndFunc) {
                    m_moveEndFunc();
                    m_moveEndFunc = nullptr;
                }
            }
		}
	}
}

void ControllerSimpleMove::nextMovePos() {
	if(m_movePosList.size() < 1) {
		return;
	}

	m_curDestPos = m_movePosList.at(0);

	m_movePosList.erase(0);
}

//按 一个坐标移动，没有人用它
void ControllerSimpleMove::moveByPos( PosBase* pos ) {
	if(m_isMoving == false && pos != NULL) {
        Vector<PosBase*> posList;
		posList.pushBack(pos);
		moveByPosList(posList);
	}
}
