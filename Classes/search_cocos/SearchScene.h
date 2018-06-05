#ifndef __SEARCH_SCENE_H__
#define __SEARCH_SCENE_H__

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "ui/UIScrollView.h"
#include "ui/UIListView.h"
#include "search/SearchInfo.h"

USING_NS_CC;
using namespace cocos2d::ui;

namespace MySearch
{

class SearchLayer : public Layer, public ui::EditBoxDelegate
{
public:
	virtual bool init() override;
	virtual void update(float delta) override;

	virtual void editBoxEditingDidBegin(EditBox* editBox)override;
	virtual void editBoxEditingDidEndWithAction(EditBox* editBox, EditBoxEndAction action)override;
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text)override;
	virtual void editBoxReturn(EditBox* editBox)override;

	void selectedItemEvent(Ref* sender, ListView::EventType type);
	//void selectedItemEventScrollView(Ref* sender, ScrollView::EventType type);
	//virtual void update(float dt) override;

	// implement the "static create()" method manually
	CREATE_FUNC(SearchLayer);

protected:
	// ���½����
	void updateResultView();
	// ��ʾ���
	void selectView(int selectId);
	void selectView(const string& input);

	// ��֡����listview
	void updateListView();

public:
	// �����ر�
	void onResultViewClose(void);

protected:
	// �����
	EditBox* inputEdit;
	// ���ҽ����
	ListView* _listView;
	// Ĭ�����
	Layout* default_item;
	// ������
	std::vector<SSimpleIndex> ResultCache;
};
}

#endif // __SEARCH_SCENE_H__
