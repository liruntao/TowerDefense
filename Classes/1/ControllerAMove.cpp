#include "ControllerAMove.h"
#include "GlobalClient.h"
#include "cocostudio/CocoStudio.h"
/*
���̣������������ȴ���һ��controsimplemove�����ctr�󶨵�һ��ʵ���ϣ�����ʱ��str��schedule��Ӧ�ÿ�ʼ�����ˣ�
����Ϊm_ismoveingΪFALSE�����Ӧ�ò�ִ���κβ�������ʵ��ȡ��һ��postlist�б�󣬵���movebypolist��������ʱ����
ispeed��spantime��������Ҫ��poslist��movebypolist�𼶵��ã���ʵ�������ֱ�������m_ismoving��Ϊture��������nextmove��
��һ����������������һ�����������ǽ��������poslist�ÿգ���ֹ��Ϊԭ���ƶ�ʱ��������ֹ�ƶ�����list��Ϊ�գ������ִ��󣩡�
�˺�update�Ϳ�ʼ���У����ú�������poslist���˳��һ��һ��moveto��destpos����Ҫ˵�����ǣ�nextmoveÿȡһ��
���꣬�͵���eraser���������Ƴ���moveonestrp��һ���ж���䣬��poslist��size����0ʱ��������δ����destpos���ŵ���moveonestep��

��a���㷨�Ժ�϶�Ҫ�Ż���
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

//���ȴ��������������m_ismovingΪtrue�����ܿ�ʼschedule��
//ȡspeed��ֵ��span��ֵ���Լ�����nextmovepos�����һ���ƶ������ֵ
void ControllerAMove::moveByPosList(Vector<PosBase*> posList) {
	if (posList.size() < 1) {
		return;
	}

	this->m_movePosList.clear();
	this->m_movePosList.pushBack(posList);
	this->nextMovePos();
}

//�Լ���ӵ�ʵ��,�϶�������
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

		if (m_fMoveTimeCount >= m_MoveSpan) {//��ʱ����ÿ֡��delta*1000����������趨���ƶ�ʱ�������򽫼�ʱ�����㣬Ȼ��moveonesetp��Ӧ�����ƶ�һ��
			m_fMoveTimeCount = 0;
			moveOneStep();
		}
	}
}

void ControllerAMove::moveOneStep() {
	//Sprite* sprite = m_entity->getSprite();//��ʼ��ʱ��������simplemove��entity���룬m_entity=entity

	//if (sprite != NULL) {
		//Armature* cur_armature = m_entity->getArmature();
		if (cur_armature!=NULL){
		Point entityPos = m_entity->getPosition();
		Point curDestPos = _layer->positionForTileCoord(tilePositionshortestPath.at(0)->getPos());
	
	


		//������Ҫ������monster������
		//this->stopActionByTag(1);
		Point diff = curDestPos - entityPos;
		if (diff.x > 0){
			m_entity->getArmature()->setScaleX(-1.0f);
		}
		else
		{
			m_entity->getArmature()->setScaleX(1.0f);
		}
		

		/* ȡ����һ���ƶ������ */
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
					/* �ƶ����� */
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
	
	
	//Sprite* sprite = m_entity->getSprite();//��ʼ��ʱ��������simplemove��entity���룬m_entity=entity

	//if (sprite != NULL) {
	


	if (cur_armature!=NULL){
		Point entityPos = m_entity->getPosition();
		Point curDestPos = m_curDestPos->getPos();
		this->moveToward(entityPos, curDestPos);
	}
}





//���������ϳ�create��init���⣬����ԭʼ�ģ�����Ϊ����ӵ�
void ControllerAMove::moveToward(const Point&fromPos, const Point &toPos){
	//û�ã���Ϊû��action
	///this->stopActionByTag(1);

	Point fromTileCoord = _layer->tileCoordForPosition(fromPos);
	Point toTileCoord = _layer->tileCoordForPosition(toPos);

	if (fromTileCoord == toTileCoord){
		log("You are already there!");
		//�����������
		this->nextMovePos();
	}
	//�±����Ӧ�ò��ü�⣬���ǻ��Ǳ���������
	if (!_layer->isValidTileCoord(toTileCoord) || _layer->isWallAtTileCoord(toTileCoord)){
		log("you can't go there forever!");
		return;
	}

//	log("From:%f,%f", fromTileCoord.x, fromTileCoord.y);
	//log("To:%f,%f", toTileCoord.x, toTileCoord.y);

	//�ÿ�open��close��ÿ�μ��㣬ÿ���ÿգ���󷵻�һ���˶��б�
	_spOpenSteps.clear();
	_spClosedSteps.clear();

	//�Ȱ���ʼ������ӵ�open�б���,
	this->insertInOpenSteps(ShortestPathStep::createWithPosition(fromTileCoord));

	do{
		// �õ���С��Fֵ����,һ��ʼ��һ�����ǳ�ʼ����
		// ��Ϊ�������б���һ������������С��Fֵ
		//���ԭ�������з��񶼵�����Ŀ�ĵأ���ô����Ȼ�ͽ������Ķ���open�б����Ƴ��ˣ�

		ShortestPathStep* currentStep = _spOpenSteps.at(0);
		_spClosedSteps.pushBack(currentStep);
		_spOpenSteps.erase(0);
		if (currentStep->gettilePosition() == toTileCoord){
			//��������Ŀ�ĵ���
			this->constructPathAndStartAnimationFromStep(currentStep);

			_spOpenSteps.clear();
			_spClosedSteps.clear();
			break;
		}
		//��õ�ǰ����Χ���п������ߵ�coord����
		PointArray *adjSteps = _layer->walkableAdjacentTilesCoordForTileCoord(currentStep->gettilePosition());
	
		for (ssize_t i = 0; i<adjSteps->count(); ++i){
			ShortestPathStep* step = ShortestPathStep::createWithPosition(adjSteps->getControlPointAtIndex(i));
			//����Ƿ���closed����
			if (this->getStepIndex(_spClosedSteps, step) != -1){
				//ֱ���������ѭ��
				continue;
			}

			//����currentstep����Ҫ�ƶ�����һ���ĳɱ�
			int moveCost = this->costToMoveFromStepToAdjacentStep(currentStep, step);
			//����ͬ�ϣ�����Ƿ���open���У����򷵻�open���е��±꣬�����򷵻�-1
			ssize_t index = this->getStepIndex(_spOpenSteps, step);
			//���ڣ�����ӵ�open����
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
					//��Ӧ�øĸ��ڵ�ɣ����ã���Ϊֱ���������step�滻��ԭ���Ǹ�step��
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

//�����������㷨�ľ��裬ÿ�β����ʱ��Ͱ�Fֵ��˳���źã�ÿ��ȡ��ʱ���ȡ��С���Ǹ�����֤ȡ�ò��������̵�·��
//���������˿��Ǻܶණ��
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

// ����ʹ�������ٷ���������ӵ�ǰ���赽��Ŀ�경�裬��ˮƽ�ʹ�ֱ�����ܵĲ���
// �����˿�����·�ϵĸ����ϰ�
int ControllerAMove::computeHScoreFromCoordToCoord(const Point &fromCoord, const Point &toCoord){
	return abs(toCoord.x - fromCoord.x) + abs(toCoord.y - fromCoord.y);
}
//��һ���赽��һ����ĳɱ������tilemap�����õ�ͼ�ɱ��Ļ��������������ã����������һ������10��б����14.
int ControllerAMove::costToMoveFromStepToAdjacentStep(const ShortestPathStep* fromStep, const ShortestPathStep* toStep){
	return ((fromStep->gettilePosition().x != toStep->gettilePosition().x)
		&& (fromStep->gettilePosition().y != toStep->gettilePosition().y)) ? 14 : 10;
}
//ȷ����һ���Ƿ���vector<shortestpathstep*>�У�open�б��close�б�������ڣ��򷵻��Ǹ��±꣬������ڣ��򷵻�-1.
ssize_t ControllerAMove::getStepIndex(const Vector<ControllerAMove::ShortestPathStep*>&steps, const ControllerAMove::ShortestPathStep *step){
	for (ssize_t i = 0; i < steps.size(); ++i){
		if (steps.at(i)->isEqual(step)){
			return i;
		}
	}
	return -1;
}

//��������̫���ʣ����ø���
//������Ӧ�ð�_shortestpath���ݳ�ȥ�ɡ���
void ControllerAMove::constructPathAndStartAnimationFromStep(ControllerAMove::ShortestPathStep* step){
	_shortestPath.clear();
	do{
		//log("step=%f", step->gettilePosition().x);
		
		if (step->getParent()){
			//������ӵ�0�����õ������
			_shortestPath.insert(0, step);
			//���ð�tilemap��shortestpath��tilemap������ӵ�shortestPathPoint��
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


//����Ϊshortestpathstep�ĺ���
ControllerAMove::ShortestPathStep::ShortestPathStep(){
	//��������汾������
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