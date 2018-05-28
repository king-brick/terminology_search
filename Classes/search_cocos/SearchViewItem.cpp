#include "SearchViewItem.h"
#include "AppMacros.h"
#include "ui/UIText.h"

using namespace MySearch;

// 输入框背景色 182,227,241
// 背景颜色228,245,250
// 字体颜色
static const Color4B TEXT_COLOR = Color4B(40, 180, 50, 255);

void initializeTextComponent(Text* text, float width, float textWidth)
{
	if (nullptr == text)
		return;

	text->setTextHorizontalAlignment(TextHAlignment::LEFT);
	text->setTextColor(TEXT_COLOR);
	text->setFontSize(TEXT_RESULT_FONTSIZE);
	text->setAnchorPoint(Vec2(0.f, 0.f));
	text->ignoreContentAdaptWithSize(true);
	text->setTextAreaSize(Size(textWidth, 0));
	text->setContentSize(Size(textWidth, 0));
}

Layout* MySearch::genDefaultItem(void)
{
	// 创建默认组件
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float width = visibleSize.width - 20;
	float textAreaWidth = width - 30;

	Text* default_text = Text::create();
	default_text->setString("Default Item");
	default_text->setName("Title Text");
	default_text->setTextHorizontalAlignment(TextHAlignment::LEFT);
	default_text->setTextColor(TEXT_COLOR);
	default_text->setFontSize(10);
	default_text->setTextAreaSize(Size(textAreaWidth, 12));
	default_text->ignoreContentAdaptWithSize(false);

	auto default_item = Layout::create();
	default_item->setTouchEnabled(true);
	default_item->setContentSize(Size(width, 12));

	default_text->setPosition(default_item->getContentSize()/2.0f);
	default_item->addChild(default_text, 1);

	return default_item;
}

bool AgencyItem::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layout::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	float width = visibleSize.width - 20;
	float height = 150.f;
	float textAreaWidth = width - 30;

	setContentSize(Size(width, height));

	float StartX = getPositionX() + 10.f;
	float StartY = getPositionY() + height - TEXT_LINE_HEIGHT;
	// 缩写
	textEnglish = Text::create();
	initializeTextComponent(textEnglish, width, textAreaWidth);
	textEnglish->setPosition(Vec2(StartX, StartY));
	addChild(textEnglish, 1);

	// 英文全称
	StartY -= TEXT_LINE_HEIGHT + 4;
	textEnglishFull = Text::create();
	initializeTextComponent(textEnglishFull, width, textAreaWidth);
	textEnglishFull->setPosition(Vec2(StartX, StartY));
	addChild(textEnglishFull, 1);

	// 中文译名
	StartY -= TEXT_LINE_HEIGHT + 4;
	textChineseFull = Text::create();
	initializeTextComponent(textChineseFull, width, textAreaWidth);
	textChineseFull->setPosition(Vec2(StartX, StartY));
	addChild(textChineseFull, 1);
	// 国家
	StartY -= TEXT_LINE_HEIGHT + 4;
	textChinese = Text::create();
	initializeTextComponent(textChinese, width, textAreaWidth);
	textChinese->setPosition(Vec2(StartX, StartY));
	addChild(textChinese, 1);

	setLayoutType(Layout::Type::VERTICAL);

	return true;
}

void AgencyItem::updateView(const SAgency& agency)
{
	textEnglish->setString(agency.Base.English);
	textChinese->setString(agency.Base.Chinese);
	textEnglishFull->setString(agency.EnglishFullName);
	textChineseFull->setString(agency.ChineseFullName);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	float line_spacing = 5;		//行间距5
								//获取所有空间的实际渲染高度总和+5*行距，然后重新设置父控件大小
	float height = textEnglish->getVirtualRendererSize().height + textChinese->getVirtualRendererSize().height + textEnglishFull->getVirtualRendererSize().height
		+ textChineseFull->getVirtualRendererSize().height + 5 * line_spacing;
	setContentSize(Size(visibleSize.width - 20, height));

	//重新设置各text位置，设置y坐标就行
	textEnglish->setPositionY(height);
	textChinese->setPositionY(height - textEnglish->getVirtualRendererSize().height - line_spacing);
	textEnglishFull->setPositionY(textChinese->getPositionY() - textChinese->getVirtualRendererSize().height - line_spacing);
	textChineseFull->setPositionY(textEnglishFull->getPositionY() - textEnglishFull->getVirtualRendererSize().height - line_spacing);
}

bool VocabularyItem::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layout::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	
	float width = visibleSize.width - 20;
	float height = 50.f;
	float textAreaWidth = width - 30;

	setContentSize(Size(width, height));

	float StartX = getPositionX() + 10.f;
	float StartY = getPositionY() + height - TEXT_LINE_HEIGHT;
	// 英文词汇
	textEnglish = Text::create();
	initializeTextComponent(textEnglish, width, textAreaWidth);
	textEnglish->setPosition(Vec2(StartX, StartY));
	addChild(textEnglish, 1);
	// 英文来源、条款号
	StartY -= TEXT_LINE_HEIGHT + 4;
	textEnglishSourceNumber = Text::create();
	initializeTextComponent(textEnglishSourceNumber, width, textAreaWidth);
	textEnglishSourceNumber->setPosition(Vec2(StartX, StartY));
	addChild(textEnglishSourceNumber, 1);
	// 中文词汇
	StartY -= TEXT_LINE_HEIGHT + 4;
	textChinese = Text::create();
	initializeTextComponent(textChinese, width, textAreaWidth);
	textChinese->setPosition(Vec2(StartX, StartY));
	addChild(textChinese, 1);
	// 中文来源、条款号
	StartY -= TEXT_LINE_HEIGHT + 4;
	textChineseSourceNumber = Text::create();
	initializeTextComponent(textChineseSourceNumber, width, textAreaWidth);
	textChineseSourceNumber->setPosition(Vec2(StartX, StartY));
	addChild(textChineseSourceNumber, 1);
	// 说明
	StartY -= TEXT_LINE_HEIGHT + 4;
	textParaphrase = Text::create();
	initializeTextComponent(textParaphrase, width, textAreaWidth);
	textParaphrase->setPosition(Vec2(StartX, StartY));
	addChild(textParaphrase, 1);

	setLayoutType(Layout::Type::VERTICAL);
	// 中文词汇
	// 中文来源、条款号
	// 说明

	return true;
}

void VocabularyItem::updateView(const SVocabulary& vocabulary)
{
	textEnglish->setString(vocabulary.Base.English);

	textEnglishSourceNumber->setString(vocabulary.EnglishSource + " : " + vocabulary.EnglishNumber);

	textChinese->setString(vocabulary.Base.Chinese);

	textChineseSourceNumber->setString(vocabulary.ChineseSource + " : " + vocabulary.ChineseNumber);

	textParaphrase->setString(vocabulary.Paraphrase);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	float line_spacing = 5;		//行间距5
								//获取所有空间的实际渲染高度总和+5*行距，然后重新设置父控件大小
	float height = textEnglish->getVirtualRendererSize().height + textEnglishSourceNumber->getVirtualRendererSize().height + textChinese->getVirtualRendererSize().height
		+ textChineseSourceNumber->getVirtualRendererSize().height + textParaphrase->getVirtualRendererSize().height + 5 * line_spacing;
	setContentSize(Size(visibleSize.width - 20, height));

	//重新设置各text位置，设置y坐标就行
	textEnglish->setPositionY(height);
	textEnglishSourceNumber->setPositionY(height - textEnglish->getVirtualRendererSize().height - line_spacing);
	textChinese->setPositionY(textEnglishSourceNumber->getPositionY() - textEnglishSourceNumber->getVirtualRendererSize().height - line_spacing);
	textChineseSourceNumber->setPositionY(textChinese->getPositionY() - textChinese->getVirtualRendererSize().height - line_spacing);
	textParaphrase->setPositionY(textChineseSourceNumber->getPositionY() - textChineseSourceNumber->getVirtualRendererSize().height - line_spacing);
}

bool CountryGroupItem::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layout::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	float width = visibleSize.width - 20;
	float height = 50.f;
	float textAreaWidth = width - 30;

	setContentSize(Size(width, height));

	float StartX = getPositionX() + 10.f;
	float StartY = getPositionY() + height - TEXT_LINE_HEIGHT;
	// 缩写
	textEnglish = Text::create();
	initializeTextComponent(textEnglish, width, textAreaWidth);
	textEnglish->setPosition(Vec2(StartX, StartY));
	addChild(textEnglish, 1);

	// 英文全称
	StartY -= TEXT_LINE_HEIGHT + 4;
	textEnglishFull = Text::create();
	initializeTextComponent(textEnglishFull, width, textAreaWidth);
	textEnglishFull->setPosition(Vec2(StartX, StartY));
	addChild(textEnglishFull, 1);

	// 中文译名
	StartY -= TEXT_LINE_HEIGHT + 4;
	textChineseFull = Text::create();
	initializeTextComponent(textChineseFull, width, textAreaWidth);
	textChineseFull->setPosition(Vec2(StartX, StartY));
	addChild(textChineseFull, 1);

	// 国家
	StartY -= TEXT_LINE_HEIGHT + 4;
	textChinese = Text::create();
	initializeTextComponent(textChinese, width, textAreaWidth);
	textChinese->setPosition(Vec2(StartX, StartY));
	addChild(textChinese, 1);

	setLayoutType(Layout::Type::VERTICAL);

	return true;
}

void CountryGroupItem::updateView(const SCountryGroup& group)
{
	textEnglish->setString(group.Base.English);
	textChinese->setString(group.Base.Chinese);
	textEnglishFull->setString(group.EnglishFullName);
	textChineseFull->setString(group.ChineseFullName);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	float line_spacing = 5;		//行间距5
								//获取所有空间的实际渲染高度总和+5*行距，然后重新设置父控件大小
	float height = textEnglish->getVirtualRendererSize().height + textChinese->getVirtualRendererSize().height + textEnglishFull->getVirtualRendererSize().height
		+ textChineseFull->getVirtualRendererSize().height + 5 * line_spacing;
	setContentSize(Size(visibleSize.width - 20, height));

	//重新设置各text位置，设置y坐标就行
	textEnglish->setPositionY(height);
	textChinese->setPositionY(height - textEnglish->getVirtualRendererSize().height - line_spacing);
	textEnglishFull->setPositionY(textChinese->getPositionY() - textChinese->getVirtualRendererSize().height - line_spacing);
	textChineseFull->setPositionY(textEnglishFull->getPositionY() - textEnglishFull->getVirtualRendererSize().height - line_spacing);
}
