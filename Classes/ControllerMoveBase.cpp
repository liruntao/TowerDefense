/*a*moveӦ��ֻ��Ҫ�ƶ�����㣬����push���ƶ�����������У��Ƚ��鷳����Ҫȡmaplayer��tilemap�Ķ�����
���Ǵ���һ��icur��Ȼ�����´���һ��tilemap������һ���Ƚ��鷳�����������������ô�죬��Ϣ���ͣ��Ѳ���Ų��ȥ�����������x��yֵ����ȥ��
position������ȥ�����԰�position��xy�ֿ�����ȥ������˵��ͬһ��tilemap���ѷ������괫��ȥ��������׳������Ǵ���������ȽϺã�

m_iCurLevel = GlobalClient::getInstance()->getiCurTollgateLevel();ȡ���е�icurlevel��

*/


#include "ControllerMoveBase.h"

ControllerMoveBase::ControllerMoveBase() 
{
    m_isMoving = false;
    m_isXLeft = false;
    m_isYUp = false;
    m_iSpeed = SPEED;
	
    m_iCheckMoveSpeed = CHECK_MOVE_SPEED_LVL2;
    m_entity = NULL;
}

ControllerMoveBase::~ControllerMoveBase() {
	
}

Point ControllerMoveBase::getNextPos( Point curPos, Point destPos ) {//Point�Ļ���Ӧ����Cocos�������
	/* �ж��ƶ����� */
    if(curPos.x > destPos.x) {
        m_isXLeft = true;
    }
    else {
        m_isXLeft = false;
    }
    if(curPos.y < destPos.y) {
        m_isYUp = true;
    }
    else {
        m_isYUp = false;
    }

	/* �����ƶ�������ٶ�ֵ�ı䵱ǰ���� */
	//����Ҳ��һ���ݴ���ƣ���ֹ��������һ���ƶ��Ĺ��󣬵�����Զ������destpos
	//���Ǽ�һ���ٶ��Ժ����������Ŀ�������꣬��ô��ֱ����Ŀ�������Ϊ�������꣬����ͬ��
	int cgSpeed = m_entity->getchangeSpeed();
	int bodyNum = m_entity->getchangebodySpeed();
	int cur_Speed = m_iSpeed + cgSpeed-bodyNum*30;
	if (cur_Speed <= 0){
		cur_Speed = 1;
	}

    if(curPos.x < destPos.x && m_isXLeft == false) {
		curPos.x += cur_Speed;
        if(curPos.x > destPos.x) {
            curPos.x = destPos.x;
        }
    }
    else if(curPos.x > destPos.x && m_isXLeft == true) {
		curPos.x -= cur_Speed;
        if(curPos.x < destPos.x) {
            curPos.x = destPos.x;
        }
    }

    if(curPos.y < destPos.y && m_isYUp == true) {
		curPos.y += cur_Speed;
        if(curPos.y > destPos.y) {
            curPos.y = destPos.y;
        }
    }
    else if(curPos.y > destPos.y && m_isYUp == false) {
		curPos.y -= cur_Speed;
        if(curPos.y < destPos.y) {
            curPos.y = destPos.y;
        }
    }
    return curPos;
}

void ControllerMoveBase::bindMoveEndFunc(std::function<void()> moveEndFunc)
{
    m_moveEndFunc = moveEndFunc;
}
