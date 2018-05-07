#ifndef __SEARCH_VIEW_ITEM_H__
#define __SEARCH_VIEW_ITEM_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"
#include "search/SearchInfo.h"

USING_NS_CC;
using namespace cocos2d::ui;

namespace MySearch
{

// 搜索结果简易信息
extern Layout* genDefaultItem(void);

// 国家认可机构名录信息
class AgencyItem : public Layout
{
public:
	virtual bool init() override;

	CREATE_FUNC(AgencyItem);

	void updateView(const SAgency& agency);

	Text* textEnglish;
	Text* textEnglishFull;
	Text* textChinese;
	Text* textChineseFull;
};

// 认可词汇信息
class VocabularyItem : public Layout
{
public:
	virtual bool init() override;

	CREATE_FUNC(VocabularyItem);

	void updateView(const SVocabulary& vocabulary);

	Text* textEnglish;
	Text* textChinese;
	Text* textEnglishSourceNumber;
	Text* textChineseSourceNumber;
	Text* textParaphrase;
};

// 国际（国家）组织信息
class CountryGroupItem : public Layout
{
public:
	virtual bool init() override;

	CREATE_FUNC(CountryGroupItem);

	void updateView(const SCountryGroup& group);

	Text* textEnglish;
	Text* textEnglishFull;
	Text* textChinese;
	Text* textChineseFull;
};

}
#endif // __SEARCH_VIEW_ITEM_H__
