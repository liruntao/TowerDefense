
#ifndef _TowerPosEditorOperateLayer_H_
#define _TowerPosEditorOperateLayer_H_

#include "cocos2d.h"

#include "editor-support/cocostudio/CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC_EXT;
USING_NS_CC;

class TowerPosEditorLayer;
class TowerPosEditorOperateLayer : public Layer {
public:
    TowerPosEditorOperateLayer();
    ~TowerPosEditorOperateLayer();

    static TowerPosEditorOperateLayer* create(TowerPosEditorLayer* layer);
    bool init(TowerPosEditorLayer* layer);

    void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated);

private:
	/* ±à¼­²ã */
    TowerPosEditorLayer* m_editorLayer;

    void outputBtnOnClick(Ref*, TouchEventType type);
    void changeModeBtnOnClick(Ref*, TouchEventType type);
    void nextLevelBtnOnClick(Ref*, TouchEventType type);
    void preLevelBtnOnClick(Ref*, TouchEventType type);

	void GotoMainScene(Ref* target, TouchEventType type);


private:
    void onDraw(const kmMat4 &transform, bool transformUpdated);
    CustomCommand _customCommand;
};

#endif
