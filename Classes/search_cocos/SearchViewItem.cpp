#include "SearchViewItem.h"
#include "AppMacros.h"
#include "ui/UIText.h"

using namespace MySearch;

Layout* MySearch::genDefaultItem(void)
{
	// 创建默认组件
	auto visibleSize = Director::getInstance()->getVisibleSize();

	Text* default_text = Text::create();
	default_text->setString("Default Item");
	default_text->setName("Title Text");
	default_text->setTextColor(Color4B::BLACK);
	default_text->setFontSize(TEXT_RESULT_FONTSIZE);
	default_text->setTextHorizontalAlignment(TextHAlignment::LEFT);

	auto default_item = Layout::create();
	default_item->setTouchEnabled(true);
	default_item->setContentSize(Size(visibleSize.width, 22));
	default_text->setPosition(Vec2(default_item->getContentSize() / 2.0f));
	default_text->setContentSize(default_item->getContentSize());
	default_item->addChild(default_text, 1);

	return default_item;
}

void initializeTextComponent(Text* text, float width, float textWidth)
{
	if (nullptr == text)
		return;

	text->setTextHorizontalAlignment(TextHAlignment::LEFT);
	text->setTextColor(Color4B::BLACK);
	text->setFontSize(TEXT_RESULT_FONTSIZE);
	text->setAnchorPoint(Vec2(0.f, 1.f));
	text->setContentSize(Size(width, TEXT_LINE_HEIGHT));
	text->ignoreContentAdaptWithSize(true);
	text->setTextAreaSize(Size(textWidth, 0));
	text->setContentSize(Size(textWidth, 0));
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

	auto anchor = Vec2(0.f, 1.f);
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

	return true;
}

void AgencyItem::updateView(const SAgency& agency)
{
	textEnglish->setString(agency.Base.English);
	textChinese->setString(agency.Base.Chinese);
	textEnglishFull->setString(agency.EnglishFullName);
	textChineseFull->setString(agency.ChineseFullName);
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
	float height = 150.f;
	float textAreaWidth = width - 30;

	auto anchor = Vec2(0.f, 1.f);

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

	auto s = textParaphrase->getVirtualRendererSize();
	float w = s.width;
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
	float height = 150.f;
	float textAreaWidth = width - 30;

	setContentSize(Size(width, height));

	auto anchor = Vec2(0.f, 1.f);
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

	return true;
}

void CountryGroupItem::updateView(const SCountryGroup& group)
{
	textEnglish->setString(group.Base.English);
	textChinese->setString(group.Base.Chinese);
	textEnglishFull->setString(group.EnglishFullName);
	textChineseFull->setString(group.ChineseFullName);
}
