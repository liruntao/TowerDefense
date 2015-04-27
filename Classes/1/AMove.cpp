#include "AMove.h"

AMove::AMove(){

}
AMove::~AMove(){

}

AMove* AMove::createWithLayer(TollgateMapLayer* layer){
	AMove* pRet = new AMove();
	if (pRet&&pRet->initWithLayer(layer)){
		pRet->autorelease();
		return pRet;
	}
	else{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool AMove::initWithLayer(TollgateMapLayer* layer){
	bool bRet = false;
	do{
		_layer = layer;
	} while (0);
	return bRet;
}

void AMove::moveToward(const Point&fromPos,const Point &toPos){
	//û�ã���Ϊû��action
	//this->stopActionByTag(1);

	Point fromTileCoord = _layer->tileCoordForPosition(fromPos);
	Point toTileCoord = _layer->tileCoordForPosition(toPos);

	if (fromTileCoord == toTileCoord){
		log("You are already there!");
		return;
	}
	//�±����Ӧ�ò��ü�⣬���ǻ��Ǳ���������
	if (!_layer->isValidTileCoord(toTileCoord) || _layer->isWallAtTileCoord(toTileCoord)){
		log("you can't go there forever!");
		return;
	}

	log("From:%f,%f", fromTileCoord.x, fromTileCoord.y);
	log("To:%f,%f", toTileCoord.x, toTileCoord.y);

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
void AMove::insertInOpenSteps(AMove::ShortestPathStep* step){
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
int AMove::computeHScoreFromCoordToCoord(const Point &fromCoord, const Point &toCoord){
	return abs(toCoord.x - fromCoord.x) + abs(toCoord.y - fromCoord.y);
}
//��һ���赽��һ����ĳɱ������tilemap�����õ�ͼ�ɱ��Ļ��������������ã����������һ������10��б����14.
int AMove::costToMoveFromStepToAdjacentStep(const ShortestPathStep* fromStep, const ShortestPathStep* toStep){
	return ((fromStep->gettilePosition().x != toStep->gettilePosition().x)
		&& (fromStep->gettilePosition().y != toStep->gettilePosition().y)) ? 14 : 10;
}
//ȷ����һ���Ƿ���vector<shortestpathstep*>�У�open�б��close�б�������ڣ��򷵻��Ǹ��±꣬������ڣ��򷵻�-1.
ssize_t AMove::getStepIndex(const Vector<AMove::ShortestPathStep*>&steps, const AMove::ShortestPathStep *step){
	for (ssize_t i = 0; i < steps.size(); ++i){
		if (steps.at(i)->isEqual(step)){
			return i;
		}
	}
	return -1;
}

//��������̫���ʣ����ø���
//������Ӧ�ð�_shortestpath���ݳ�ȥ�ɡ���
void AMove::constructPathAndStartAnimationFromStep(AMove::ShortestPathStep* step){
	_shortestPath.clear();
	do{
		if (step->getParent()){
			//������ӵ�0�����õ������
			_shortestPath.insert(0, step);
		}
		step = step->getParent();

	} while (step);
	// ��·������ӡ����
	for (const ShortestPathStep *s : _shortestPath){
		log("%s", s->getDescription().c_str());
	}
	//û����ʱ
	//this->popStepAndAnimate();
}
//��������ò��ϣ�����
void AMove::popStepAndAnimate(){

}

//����Ϊshortestpathstep�ĺ���
AMove::ShortestPathStep::ShortestPathStep(){
	//��������汾������
	//_tileposition(Point::ZERO);
	//_gScore(0);
	//_hScore(0);
	//_parent(nullptr);
}
AMove::ShortestPathStep::~ShortestPathStep(){

}
AMove::ShortestPathStep *AMove::ShortestPathStep::createWithPosition(const Point &pos){
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
bool AMove::ShortestPathStep::initWithPosition(const Point &pos){
	bool bRet = false;
	do{
		this->settilePosition(pos);
		bRet = true;
	} while (0);
	return bRet;
}
int AMove::ShortestPathStep::getFScore()const{
	return this->getGScore() + this->getHScore();
}

bool AMove::ShortestPathStep::isEqual(const AMove::ShortestPathStep* other) const{
	return this->gettilePosition() == other->gettilePosition();
}

std::string AMove::ShortestPathStep::getDescription()const
{
	return StringUtils::format("pos=[%.0f;%.0f] g=%d h=%d f=%d",
		this->gettilePosition().x, this->gettilePosition().y,
		this->getGScore(), this->getHScore(), this->getFScore());
}