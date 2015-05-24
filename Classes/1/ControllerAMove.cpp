#include "ControllerAMove.h"
#include "GlobalClient.h"
#include "cocostudio/CocoStudio.h"
/*
流程：在其他类里先创建一个controsimplemove，这个ctr绑定到一个实体上，（此时，str的schedule就应该开始调用了，
但因为m_ismoveing为FALSE，因此应该不执行任何操作）该实体取得一个postlist列表后，调用movebypolist函数，此时传入
ispeed，spantime，和最重要到poslist，movebypolist逐级调用，并实例化各种变量，将m_ismoving置为ture，并开启nextmove的
第一步操作（这里尚有一个操作，就是将此类里的poslist置空，防止因为原来移动时，由于终止移动导致list不为空，而出现错误）。
此后，update就开始运行，调用函数按照poslist里的顺序，一步一步moveto到destpos（需要说明的是，nextmove每取一个
坐标，就调用eraser将此坐标移除。moveonestrp有一个判断语句，当poslist的size大于0时，代表尚未到达destpos，才调用moveonestep）

加a星算法以后肯定要优化。
*/


ControllerAMove::ControllerAMove() :
_facingForwardAnimation(nullptr),
_facingBackAnimation(nullptr),
_facingLeftAnimation(nullptr),
_facingRightAnimation(nullptr),

cur_Animate(nullptr)
{
	m_fMoveTimeCount = 0;
	m_MoveSpan = 0;
}

ControllerAMove::~ControllerAMove() {
	CC_SAFE_RELEASE_NULL(_facingForwardAnimation);
	CC_SAFE_RELEASE_NULL(_facingBackAnimation);
	CC_SAFE_RELEASE_NULL(_facingLeftAnimation);
	CC_SAFE_RELEASE_NULL(_facingRightAnimation);
}


ControllerAMove* ControllerAMove::createWithLayer(Entity* entity, TollgateMapLayer* layer) {
	ControllerAMove* ctrlSimpleMove = new ControllerAMove();

	if (ctrlSimpleMove && ctrlSimpleMove->initWithLayer(entity,layer)) {
		ctrlSimpleMove->autorelease();
	}
	else {
		CC_SAFE_DELETE(ctrlSimpleMove);
	}
	return ctrlSimpleMove;
}


bool ControllerAMove::initWithLayer(Entity* entity, TollgateMapLayer* layer) {
	this->m_entity = entity;
	_layer = layer;
	
	_facingForwardAnimation = this->createAnimation("forward");
	_facingForwardAnimation->retain();
	_facingBackAnimation = this->createAnimation("back");
	_facingBackAnimation->retain();
	_facingLeftAnimation = this->createAnimation("left");
	_facingLeftAnimation->retain();
	_facingRightAnimation = this->createAnimation("right");
	_facingRightAnimation->retain();

	cur_armature = m_entity->getArmature();
	this->schedule(schedule_selector(ControllerAMove::checkMoveUpdate));

	return true;
}

//首先传入这个函数，置m_ismoving为true，才能开始schedule。
//取speed的值，span的值，以及调用nextmovepos获得下一步移动坐标的值
void ControllerAMove::moveByPosList(Vector<PosBase*> posList) {
	if (posList.size() < 1) {
		return;
	}

	this->m_movePosList.clear();
	this->m_movePosList.pushBack(posList);
	this->nextMovePos();
}

//自己添加的实验,肯定有问题
Animation *ControllerAMove::createAnimation(const char *animType)
{
	Animation *animation = Animation::create();
	for (int i = 1; i <= 2; ++i)
	{
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("cat_%s_%d.png", animType, i)));
		log("1");
	}
	animation->setDelayPerUnit(0.2f);
	return animation;
}


void ControllerAMove::moveByPosList(Vector<PosBase*> posList, int iSpeed) {
	this->m_iSpeed = iSpeed;
	moveByPosList(posList);
}

void ControllerAMove::moveByPosList(Vector<PosBase*> posList, int iSpeed, int iSpanTime) {
	m_MoveSpan = iSpanTime;
	moveByPosList(posList, iSpeed);
}
void ControllerAMove::checkMoveUpdate(float delta) {
	if (m_isMoving) {
		m_fMoveTimeCount += delta * 100000;

		if (m_fMoveTimeCount >= m_MoveSpan) {//计时器，每帧加delta*1000，如果大于设定的移动时间间隔，则将计时器置零，然后moveonesetp，应该是移动一步
			m_fMoveTimeCount = 0;
			moveOneStep();
		}
	}
}

void ControllerAMove::moveOneStep() {
	//Sprite* sprite = m_entity->getSprite();//初始化时，将调用simplemove的entity传入，m_entity=entity

	//if (sprite != NULL) {
		//Armature* cur_armature = m_entity->getArmature();
		if (cur_armature!=NULL){
		Point entityPos = m_entity->getPosition();
		Point curDestPos = _layer->positionForTileCoord(tilePositionshortestPath.at(0)->getPos());
	
	


		//动画，要传进来monster的种类
		//this->stopActionByTag(1);
		Point diff = curDestPos - entityPos;
		if (diff.x > 0){
			m_entity->getArmature()->setScaleX(-1.0f);
		}
		else
		{
			m_entity->getArmature()->setScaleX(1.0f);
		}
		

		/* 取得下一个移动坐标点 */
		entityPos = getNextPos(entityPos, curDestPos);
		m_entity->setPosition(entityPos);
	
		if (entityPos.x == curDestPos.x&&entityPos.y == curDestPos.y){
			if (tilePositionshortestPath.size() > 1){
				tilePositionshortestPath.erase(0);
			}
			else
			{
				if (m_movePosList.size() > 0) {
					m_isMoving = false;
					tilePositionshortestPath.clear();


					nextMovePos();
					//MoveTo *moveAction = MoveTo::create(0.4f, _layer->positionForTileCoord(s->gettilePosition()));
					//CallFunc *moveCallback = CallFunc::create(CC_CALLBACK_0(CatSprite::popStepAndAnimate, this));
					//Sequence *moveSequence = Sequence::create(nextMovePos);
				//	moveSequence->setTag(1);
					//this->runAction(moveSequence);


					
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
}
			



void ControllerAMove::nextMovePos() {
	if (m_movePosList.size() < 0) {
		m_isMoving = false;
		return;
	}
	m_curDestPos = m_movePosList.at(0);
	m_movePosList.erase(0);
	
	
	//Sprite* sprite = m_entity->getSprite();//初始化时，将调用simplemove的entity传入，m_entity=entity

	//if (sprite != NULL) {
	


	if (cur_armature!=NULL){
		Point entityPos = m_entity->getPosition();
		Point curDestPos = m_curDestPos->getPos();
		this->moveToward(entityPos, curDestPos);
	}
}





//照例，以上除create和init以外，都是原始的，以下为新添加的
void ControllerAMove::moveToward(const Point&fromPos, const Point &toPos){
	//没用，因为没有action
	///this->stopActionByTag(1);

	Point fromTileCoord = _layer->tileCoordForPosition(fromPos);
	Point toTileCoord = _layer->tileCoordForPosition(toPos);

	if (fromTileCoord == toTileCoord){
		log("You are already there!");
		//这估计有问题
		this->nextMovePos();
	}
	//下边这个应该不用检测，但是还是保留下来吧
	if (!_layer->isValidTileCoord(toTileCoord) || _layer->isWallAtTileCoord(toTileCoord)){
		log("you can't go there forever!");
		return;
	}

//	log("From:%f,%f", fromTileCoord.x, fromTileCoord.y);
	//log("To:%f,%f", toTileCoord.x, toTileCoord.y);

	//置空open与close表，每次计算，每次置空，最后返回一个运动列表。
	_spOpenSteps.clear();
	_spClosedSteps.clear();

	//先把起始坐标添加到open列表中,
	this->insertInOpenSteps(ShortestPathStep::createWithPosition(fromTileCoord));

	do{
		// 得到最小的F值步骤,一开始第一个就是初始坐标
		// 因为是有序列表，第一个步骤总是最小的F值
		//如果原来的所有方格都到不到目的地，那么，自然就将其他的都从open列表中移除了，

		ShortestPathStep* currentStep = _spOpenSteps.at(0);
		_spClosedSteps.pushBack(currentStep);
		_spOpenSteps.erase(0);
		if (currentStep->gettilePosition() == toTileCoord){
			//表明到达目的地了
			this->constructPathAndStartAnimationFromStep(currentStep);

			_spOpenSteps.clear();
			_spClosedSteps.clear();
			break;
		}
		//获得当前步周围所有可以行走的coord方块
		PointArray *adjSteps = _layer->walkableAdjacentTilesCoordForTileCoord(currentStep->gettilePosition());
	
		for (ssize_t i = 0; i<adjSteps->count(); ++i){
			ShortestPathStep* step = ShortestPathStep::createWithPosition(adjSteps->getControlPointAtIndex(i));
			//检查是否在closed表中
			if (this->getStepIndex(_spClosedSteps, step) != -1){
				//直接跳出这次循环
				continue;
			}

			//计算currentstep到将要移动的这一步的成本
			int moveCost = this->costToMoveFromStepToAdjacentStep(currentStep, step);
			//步骤同上，检查是否在open表中，在则返回open表中的下标，不在则返回-1
			ssize_t index = this->getStepIndex(_spOpenSteps, step);
			//不在，则添加到open表中
			if (index == -1){
				step->setParent(currentStep);
				step->setGScore(currentStep->getGScore() + moveCost);
				step->setHScore(this->computeHScoreFromCoordToCoord(step->gettilePosition(), toTileCoord));
				this->insertInOpenSteps(step);
			}
			else
			{
				step = _spOpenSteps.at(index);
				if (currentStep->getGScore() + moveCost < step->getGScore())
				{
					step->setGScore(currentStep->getGScore() + moveCost);
					step->retain();
					_spOpenSteps.erase(index);
					//它应该改父节点吧，不用，因为直接是用这个step替换掉原来那个step了
					//step->setParent(currentStep);
					this->insertInOpenSteps(step);
					step->release();
				}
			}
		}
	} while (_spOpenSteps.size() > 0);

	//if (_shortestPath.empty())
	//{
	//	return;
	//}

}

//这个才是这个算法的精髓，每次插入的时候就按F值的顺序排好，每次取的时候就取最小的那个，保证取得差不多算是最短的路径
//这样避免了考虑很多东西
void ControllerAMove::insertInOpenSteps(ControllerAMove::ShortestPathStep* step){
	int stepFScore = step->getFScore();
	ssize_t	count = _spOpenSteps.size();
	ssize_t i = 0;
	for (; i < count; ++i){
		if (stepFScore <= _spOpenSteps.at(i)->getFScore()){
			break;
		}
	}
	_spOpenSteps.insert(i, step);
}

// 这里使用曼哈顿方法，计算从当前步骤到达目标步骤，在水平和垂直方向总的步数
// 忽略了可能在路上的各种障碍
int ControllerAMove::computeHScoreFromCoordToCoord(const Point &fromCoord, const Point &toCoord){
	return abs(toCoord.x - fromCoord.x) + abs(toCoord.y - fromCoord.y);
}
//上一步骤到下一步骤的成本，如果tilemap里设置地图成本的话，就在这里设置，这里仅仅是一个格是10，斜格是14.
int ControllerAMove::costToMoveFromStepToAdjacentStep(const ShortestPathStep* fromStep, const ShortestPathStep* toStep){
	return ((fromStep->gettilePosition().x != toStep->gettilePosition().x)
		&& (fromStep->gettilePosition().y != toStep->gettilePosition().y)) ? 14 : 10;
}
//确定这一步是否在vector<shortestpathstep*>中（open列表和close列表），如果在，则返回那个下标，如果不在，则返回-1.
ssize_t ControllerAMove::getStepIndex(const Vector<ControllerAMove::ShortestPathStep*>&steps, const ControllerAMove::ShortestPathStep *step){
	for (ssize_t i = 0; i < steps.size(); ++i){
		if (steps.at(i)->isEqual(step)){
			return i;
		}
	}
	return -1;
}

//函数名不太合适，懒得改了
//在这里应该把_shortestpath传递出去吧。。
void ControllerAMove::constructPathAndStartAnimationFromStep(ControllerAMove::ShortestPathStep* step){
	_shortestPath.clear();
	do{
		//log("step=%f", step->gettilePosition().x);
		
		if (step->getParent()){
			//总是添加到0，正好倒序添加
			_shortestPath.insert(0, step);
			//正好把tilemap的shortestpath的tilemap坐标添加到shortestPathPoint里
			PosBase* s = PosBase::create(step->gettilePosition());
			tilePositionshortestPath.insert(0, s);
		}
		step = step->getParent();

	} while (step);
	this->m_isMoving=true;

	for (const ShortestPathStep *s : _shortestPath){
		
		log("%s", s->getDescription().c_str());
	}

}


//以下为shortestpathstep的函数
ControllerAMove::ShortestPathStep::ShortestPathStep(){
	//好像这个版本不能用
	//_tileposition(Point::ZERO);
	_gScore=0;
	_hScore=0;
	_parent=NULL;
}
ControllerAMove::ShortestPathStep::~ShortestPathStep(){

}
ControllerAMove::ShortestPathStep *ControllerAMove::ShortestPathStep::createWithPosition(const Point &pos){
	ShortestPathStep* pRet = new ShortestPathStep();
	if (pRet&&pRet->initWithPosition(pos)){
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}
bool ControllerAMove::ShortestPathStep::initWithPosition(const Point &pos){
	bool bRet = false;
	do{
		this->settilePosition(pos);
		bRet = true;
	} while (0);
	return bRet;
}
int ControllerAMove::ShortestPathStep::getFScore()const{
	return this->getGScore() + this->getHScore();
}

bool ControllerAMove::ShortestPathStep::isEqual(const ControllerAMove::ShortestPathStep* other) const{
	return this->gettilePosition() == other->gettilePosition();
}

std::string ControllerAMove::ShortestPathStep::getDescription()const
{
	return StringUtils::format("pos=[%.0f;%.0f] g=%d h=%d f=%d",
		this->gettilePosition().x, this->gettilePosition().y,
		this->getGScore(), this->getHScore(), this->getFScore());
}