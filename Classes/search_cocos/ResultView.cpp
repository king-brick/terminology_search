#include "ResultView.h"
#include "AppMacros.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "search/SearchInfo.h"
#include "SearchViewItem.h"
#include "SearchScene.h"

using namespace MySearch;

Scene* ResultViewLayer::scene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	ResultViewLayer *layer = ResultViewLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool ResultViewLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
	Button* button = Button::create("btn_back.png");
	button->setPosition(Vec2(origin.x + visibleSize.width - button->getContentSize().width / 2 - 15, origin.y + visibleSize.height - button->getContentSize().height / 2));
	//button->setTitleText("BACK");
	button->addTouchEventListener(CC_CALLBACK_2(ResultViewLayer::touchEvent, this));
	addChild(button, 2);

	// 创建索引框
	_listView = ListView::create();
	// set list view ex direction
	_listView->setDirection(ScrollView::Direction::VERTICAL);
	_listView->setBounceEnabled(true);
	_listView->setBackGroundImage("bk_output.png");
	_listView->setBackGroundImageScale9Enabled(true);
	_listView->setContentSize(visibleSize);
	_listView->setPosition(Vec2(origin.x + (visibleSize.width - _listView->getContentSize().width) / 2,
		origin.y + visibleSize.height - _listView->getContentSize().height - button->getContentSize().height));
	_listView->setScrollBarPositionFromCorner(Vec2(7, 7));
	// set all items layout gravity
	_listView->setGravity(ListView::Gravity::CENTER_VERTICAL);
	addChild(_listView, 1);

    return true;
}

void ResultViewLayer::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
	{
		SearchLayer* layer = (SearchLayer*)getParent();
		if (nullptr != layer)
		{
			layer->onResultViewClose();
		}
		// 关闭该页面
		removeFromParent();
	}
	break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void ResultViewLayer::updateResultView(int selectId)
{
	const SVocabulary& vocabulary = g_Dictionary.GetVocabularyInfo(selectId);
	VocabularyItem* item = VocabularyItem::create();
	if (nullptr != item)
	{
		item->updateView(vocabulary);
		_listView->pushBackCustomItem(item);
	}

	// set items margin
	float spacing = 4;
	_listView->setItemsMargin(spacing);
	_listView->jumpToBottom();

	//update listview data
	this->scheduleUpdate();

	_listView->forceDoLayout();
}

void ResultViewLayer::updateResultView(const string input)
{
	const SSimpleIndex* result = g_Dictionary.GetSearchIndex(input);
	if (nullptr == result)
		return;

	int nIndex = 0;
	for (auto idx : result->ids)
	{
		//// 增加一条间隔线
		//if (0 < nIndex)
		//{
		//	CCSprite* sprite = CCSprite::createWithSpriteFrameName("orange_edit.png");
		//	if (nullptr != sprite)
		//	{
		//		_listView->pushBackCustomItem(sprite);
		//	}
		//}

		switch (idx.Type)
		{
		case EType::Vocabulary:
		{
			VocabularyItem* item = VocabularyItem::create();
			if (nullptr != item)
			{
				const SVocabulary& vocabulary = g_Dictionary.GetVocabularyInfo(idx.Id);
				item->updateView(vocabulary);
				_listView->pushBackCustomItem(item);
			}
		}
		break;
		case EType::Agency:
		{
			AgencyItem* item = AgencyItem::create();
			if (nullptr != item)
			{
				const SAgency& agency = g_Dictionary.GetAgencyInfo(idx.Id);
				item->updateView(agency);
				_listView->pushBackCustomItem(item);
			}
		}
		break;
		case EType::Group:
		{
			CountryGroupItem* item = CountryGroupItem::create();
			if (nullptr != item)
			{
				const SCountryGroup& group = g_Dictionary.GetGroupInfo(idx.Id);
				item->updateView(group);
				_listView->pushBackCustomItem(item);
			}
		}
		break;
		default:
			break;
		}

		nIndex++;
	}

	// set items margin
	float spacing = 4;
	_listView->setItemsMargin(spacing);
	//_listView->jumpToBottom();

	//update listview data
	this->scheduleUpdate();

	_listView->forceDoLayout();
}
