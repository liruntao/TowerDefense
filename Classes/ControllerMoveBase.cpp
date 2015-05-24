/*a*move应该只需要移动坐标点，将其push到移动坐标的容器中，比较麻烦的是要取maplayer里tilemap的东西。
还是传入一个icur，然后重新创建一个tilemap？还有一个比较麻烦的是如果创建塔了怎么办，消息派送，把不能挪过去的塔的坐标的x，y值传过去？
position传不过去，可以把position的xy分开传过去。或者说，同一个tilemap，把方块坐标传过去（这个容易出错，还是传世界坐标比较好）

m_iCurLevel = GlobalClient::getInstance()->getiCurTollgateLevel();取现有的icurlevel。

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

Point ControllerMoveBase::getNextPos( Point curPos, Point destPos ) {//Point的话，应该是Cocos的坐标吧
	/* 判断移动方向 */
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

	/* 根据移动方向和速度值改变当前坐标 */
	//这里也算一个容错机制，防止出现由于一次移动的过大，导致永远到不了destpos
	//就是加一次速度以后，如果超过了目标点的坐标，那么就直接置目标点坐标为现在坐标，以下同理
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
