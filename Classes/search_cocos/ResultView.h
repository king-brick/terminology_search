#ifndef __RESULT_VIEW_H__
#define __RESULT_VIEW_H__

#include "cocos2d.h"
#include "ui/UIListView.h"
#include "search/SearchInfo.h"

USING_NS_CC;
using namespace cocos2d::ui;

namespace MySearch
{

class ResultViewLayer : public Layer
{
public:
	virtual bool init() override;

	static cocos2d::Scene* scene();

	// implement the "static create()" method manually
	CREATE_FUNC(ResultViewLayer);

	// 更新结果框
	void updateResultView(int selectId);
	void updateResultView(const string input);

	void touchEvent(Ref *pSender, Widget::TouchEventType type);

protected:
	// 查找结果框
	ListView* _listView;
};
}

#endif // __RESULT_VIEW_H__
