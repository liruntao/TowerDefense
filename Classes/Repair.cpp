#include "Repair.h"

bool Repair::init(int totalBlood)
{
	if (!Layer::init())
	{
		return false;
	}
	counter = 1;
	percent = 100;
	repairTime = totalBlood;

	auto progressBarBg = Sprite::create("progressBg.png");
	progressBarBg->setAnchorPoint(Point(0, 0));
	progressBarBg->setPosition(Point(0, 0));
	addChild(progressBarBg);

	progressBar = ProgressTimer::create(Sprite::create("progressBar.png"));
	progressBar->setType(ProgressTimer::Type::BAR);
	progressBar->setMidpoint(Point(0, 0.5f));
	progressBar->setBarChangeRate(Point(1, 0));
	progressBar->setPercentage(percent);
	progressBar->setAnchorPoint(Point(0.5f, 0.5f));
	progressBar->setPosition(Point(progressBarBg->getContentSize().width / 2, progressBarBg->getContentSize().height / 2));
	progressBarBg->addChild(progressBar);

	nameLabel = Label::createWithBMFont("fonts/Font30.fnt", "");
	nameLabel->setPosition(Point(progressBarBg->getContentSize().width / 2, progressBarBg->getContentSize().height));
	nameLabel->setAnchorPoint(Point(0.5f, 0.5f));
	progressBarBg->addChild(nameLabel);

	timeLabel = Label::createWithBMFont("fonts/Font30.fnt", "");
	timeLabel->setPosition(Point(progressBarBg->getContentSize().width / 2, -progressBarBg->getContentSize().height));
	timeLabel->setAnchorPoint(Point(0.5f, 0.5f));
	progressBarBg->addChild(timeLabel);
	this->schedule(schedule_selector(Repair::updateProgressBar), 1.0f);

	return true;

}

Repair* Repair::create(Point pos,int totalBlood)
{
	Repair* pRet = new Repair();
	if (pRet&&pRet->init(totalBlood))
	{
		pRet->setRepairPos(pos);
		
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

void Repair::updateProgressBar(float dt)
{
	counter++;
	percent = (float(repairTime) - float(counter)) / float(repairTime) * 100;

	if (percent <= 100 && percent > 0)
	{
		progressBar->setPercentage(percent);
		timeLabel->setString(std::to_string(repairTime - counter) + "miao");
	}
	else
	{
		this->setTimeOut(true);
		this->unschedule(schedule_selector(Repair::updateProgressBar));
		this->removeFromParent();
	}
}