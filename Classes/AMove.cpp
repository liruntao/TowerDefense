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
	//没用，因为没有action
	//this->stopActionByTag(1);

	Point fromTileCoord = _layer->tileCoordForPosition(fromPos);
	Point toTileCoord = _layer->tileCoordForPosition(toPos);

	if (fromTileCoord == toTileCoord){
		log("You are already there!");
		return;
	}
	//下边这个应该不用检测，但是还是保留下来吧
	if (!_layer->isValidTileCoord(toTileCoord) || _layer->isWallAtTileCoord(toTileCoord)){
		log("you can't go there forever!");
		return;
	}

	log("From:%f,%f", fromTileCoord.x, fromTileCoord.y);
	log("To:%f,%f", toTileCoord.x, toTileCoord.y);

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

// 这里使用曼哈顿方法，计算从当前步骤到达目标步骤，在水平和垂直方向总的步数
// 忽略了可能在路上的各种障碍
int AMove::computeHScoreFromCoordToCoord(const Point &fromCoord, const Point &toCoord){
	return abs(toCoord.x - fromCoord.x) + abs(toCoord.y - fromCoord.y);
}
//上一步骤到下一步骤的成本，如果tilemap里设置地图成本的话，就在这里设置，这里仅仅是一个格是10，斜格是14.
int AMove::costToMoveFromStepToAdjacentStep(const ShortestPathStep* fromStep, const ShortestPathStep* toStep){
	return ((fromStep->gettilePosition().x != toStep->gettilePosition().x)
		&& (fromStep->gettilePosition().y != toStep->gettilePosition().y)) ? 14 : 10;
}
//确定这一步是否在vector<shortestpathstep*>中（open列表和close列表），如果在，则返回那个下标，如果不在，则返回-1.
ssize_t AMove::getStepIndex(const Vector<AMove::ShortestPathStep*>&steps, const AMove::ShortestPathStep *step){
	for (ssize_t i = 0; i < steps.size(); ++i){
		if (steps.at(i)->isEqual(step)){
			return i;
		}
	}
	return -1;
}

//函数名不太合适，懒得改了
//在这里应该把_shortestpath传递出去吧。。
void AMove::constructPathAndStartAnimationFromStep(AMove::ShortestPathStep* step){
	_shortestPath.clear();
	do{
		if (step->getParent()){
			//总是添加到0，正好倒序添加
			_shortestPath.insert(0, step);
		}
		step = step->getParent();

	} while (step);
	// 把路径都打印出来
	for (const ShortestPathStep *s : _shortestPath){
		log("%s", s->getDescription().c_str());
	}
	//没用暂时
	//this->popStepAndAnimate();
}
//这个现在用不上，留存
void AMove::popStepAndAnimate(){

}

//以下为shortestpathstep的函数
AMove::ShortestPathStep::ShortestPathStep(){
	//好像这个版本不能用
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