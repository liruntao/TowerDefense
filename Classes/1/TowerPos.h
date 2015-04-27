/*
    文件名：    TowerPos.h
    描　述：    炮塔坐标对象
    创建人：    笨木头_钟迪龙 (博客：http://www.benmutou.com)
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

    /* 判断坐标是否进入范围 */
    virtual bool isClickMe(Point pos) override;

private:
  //  void onDraw(const kmMat4 &transform, bool transformUpdated);
 //   CustomCommand _customCommand;
};

#endif