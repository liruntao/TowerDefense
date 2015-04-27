/*
    �ļ�����    TowerPos.h
    �衡����    �����������
    �����ˣ�    ��ľͷ_�ӵ��� (���ͣ�http://www.benmutou.com)
*/

#ifndef _TowerPos_H_
#define _TowerPos_H_

#include "PosBase.h"
#include "cocos2d.h"
USING_NS_CC;

#define RADIUS 32

class TowerPos : public PosBase {
public:
    TowerPos();
    ~TowerPos();
	static TowerPos* create(Point pos);
	static TowerPos* create(Point pos, int herotype, int bullettype, bool isDebug);
	bool init(Point pos);
	bool init(Point pos, int herotype,int bullettype,  bool isDebug);



 //   void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated);

    /* �ж������Ƿ���뷶Χ */
    virtual bool isClickMe(Point pos) override;

private:
  //  void onDraw(const kmMat4 &transform, bool transformUpdated);
 //   CustomCommand _customCommand;
};

#endif