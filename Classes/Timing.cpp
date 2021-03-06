//
//  Timing.cpp
//  SLG
//
//  Created by renshan on 14/12/31.
//
//

#include "Timing.h"




bool TimingLayer::init()
{
    if (!Layer::init())
	{
		return false;
	}
    counter= 1;
    percent = 100;
	auto progressBarBg = Sprite::create("progressBg.png");
    progressBarBg->setAnchorPoint(Point(0, 0));
	progressBarBg->setPosition(Point(0,  0 ));
	addChild(progressBarBg);
    
    progressBar = ProgressTimer::create(Sprite::create("progressBar.png"));
	progressBar->setType(ProgressTimer::Type::BAR);
	progressBar->setMidpoint(Point(0, 0.5f));
	progressBar->setBarChangeRate(Point(1, 0));
	progressBar->setPercentage(percent);
	progressBar->setAnchorPoint(Point(0.5f, 0.5f));
    progressBar->setPosition(Point(progressBarBg->getContentSize().width / 2,  progressBarBg->getContentSize().height /2 ));
	progressBarBg->addChild(progressBar);
    
    nameLabel = Label::createWithBMFont("fonts/Font30.fnt"," ");
    nameLabel->setPosition(Point(progressBarBg->getContentSize().width / 2,  progressBarBg->getContentSize().height ));
	nameLabel->setAnchorPoint(Point(0.5f, 0.0f));
    progressBarBg->addChild(nameLabel);
    
    timeLabel = Label::createWithBMFont("fonts/Font30.fnt","");
    timeLabel->setPosition(Point(progressBarBg->getContentSize().width / 2,  - progressBarBg->getContentSize().height ));
    timeLabel->setAnchorPoint(Point(0.5f, 0.0f));
    progressBarBg->addChild(timeLabel);
    this->schedule(schedule_selector(TimingLayer::updateProgressBar), 1.0f);



    return true;
}

//这里可以用一个表，读外部的数据。
void TimingLayer::setParam(CropsType type)
{
    switch (type) {
        case WHEAT:
            nameLabel->setString("小麦");
            growUpTime = 60;
            break;
        case CORN:
            nameLabel->setString("玉米");
            growUpTime = 120;
            break;
        case CARROT:
         //   nameLabel->setString("胡萝卜");
			nameLabel->setString("huluobo");

            growUpTime = 180;
            break;
            
        default:
            break;
    }
}
TimingLayer* TimingLayer::create(Point pos, CropsType type)
{
    TimingLayer *pRet = new TimingLayer();
    if (pRet && pRet->init())
    {
        pRet->setTimingLayerPos(pos);
        pRet->setParam(type);
        
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }

}

void TimingLayer::updateProgressBar(float dt)
{
    counter++;
    percent = 100 - float(counter) / float(growUpTime ) * 100;
    if(percent <= 100 && percent > 0 )
    {
        progressBar->setPercentage(percent);
   //     timeLabel ->setString(std::to_string(growUpTime - counter) + " 秒");
		timeLabel->setString(std::to_string(growUpTime - counter) + "miao");

    }
    else
    {
        this->setTimeOut( true);
        this->unschedule(schedule_selector(TimingLayer::updateProgressBar));
        this->removeFromParent();

		//在这里设计如果时间到了以后会怎么样

    }
}


