/*
���̣������������ȴ���һ��controsimplemove�����ctr�󶨵�һ��ʵ���ϣ�����ʱ��str��schedule��Ӧ�ÿ�ʼ�����ˣ�
����Ϊm_ismoveingΪFALSE�����Ӧ�ò�ִ���κβ�������ʵ��ȡ��һ��postlist�б�󣬵���movebypolist��������ʱ����
ispeed��spantime��������Ҫ��poslist��movebypolist�𼶵��ã���ʵ�������ֱ�������m_ismoving��Ϊture��������nextmove��
��һ����������������һ�����������ǽ��������poslist�ÿգ���ֹ��Ϊԭ���ƶ�ʱ��������ֹ�ƶ�����list��Ϊ�գ������ִ��󣩡�
�˺�update�Ϳ�ʼ���У����ú�������poslist���˳��һ��һ��moveto��destpos����Ҫ˵�����ǣ�nextmoveÿȡһ��
���꣬�͵���eraser���������Ƴ���moveonestrp��һ���ж���䣬��poslist��size����0ʱ��������δ����destpos���ŵ���moveonestep��


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

//���ȴ��������������m_ismovingΪtrue�����ܿ�ʼschedule��
//ȡspeed��ֵ��span��ֵ���Լ�����nextmovepos�����һ���ƶ������ֵ
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

        if(m_fMoveTimeCount >= m_MoveSpan) {//��ʱ����ÿ֡��delta*1000����������趨���ƶ�ʱ�������򽫼�ʱ�����㣬Ȼ��moveonesetp��Ӧ�����ƶ�һ��
            m_fMoveTimeCount = 0;
		    moveOneStep();
        }
	}
}

void ControllerSimpleMove::moveOneStep() {
	Sprite* sprite = m_entity->getSprite();//��ʼ��ʱ��������simplemove��entity���룬m_entity=entity

	if(sprite != NULL) {
		Point entityPos = m_entity->getPosition();
		Point curDestPos = m_curDestPos->getPos();//����Ϊʲô��getpos����û�ж��壿
		
		/* ȡ����һ���ƶ������ */
        entityPos = getNextPos(entityPos, curDestPos);

		/* �ƶ�ʵ�� */
		m_entity->setPosition(entityPos);

		/* ���ﵱǰĿ�ĵأ���ʼ��һ��Ŀ�ĵ� */
		if(entityPos.x == curDestPos.x && entityPos.y == curDestPos.y) {
			if(m_movePosList.size() > 0) {
				nextMovePos();
			}
            else {
                /* �ƶ����� */
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

//�� һ�������ƶ���û��������
void ControllerSimpleMove::moveByPos( PosBase* pos ) {
	if(m_isMoving == false && pos != NULL) {
        Vector<PosBase*> posList;
		posList.pushBack(pos);
		moveByPosList(posList);
	}
}
