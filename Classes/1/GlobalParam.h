#ifndef __GLOBAL_PARAM_H__
#define __GLOBAL_PARAM_H__

#include "cocos2d.h"

using namespace cocos2d;

class GlobalParam {
public:
    /* 开发时默认的屏幕大小 */
    static const int DEFAULT_SCREEN_WIDTH = 1024;
    static const int DEFAULT_SCREEN_HEIGHT = 640;

    /* 字体 */
    static const int PublicFontSize = 22;
    static const int PublicFontSizeLarge = 35;
    static const int PublicFontSizeLargest = 60;

    /* Layer层次 */
    static const int LayerOrder_LvLLowest = 1;
    static const int LayerOrder_LvL11 = 20;
    static const int LayerOrder_LvL9 = 30;
    static const int LayerOrder_LvL7 = 40;
    static const int LayerOrder_LvLNormal = 50;
    static const int LayerOrder_LvL3 = 60;
    static const int LayerOrder_LvL1 = 70;
    static const int LayerOrder_LvLHighest = 200;

    /* 角色ID最大位数 */
    static const int ACTOR_ID_MAX_NUM = 8;
};
#endif