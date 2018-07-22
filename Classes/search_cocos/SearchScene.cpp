#include "SearchScene.h"
#include "AppMacros.h"
#include "ui/UIText.h"
#include "ui/UIImageView.h"
#include "ResultView.h"
#include "SearchViewItem.h"
#include <algorithm>

using namespace MySearch;

// on "init" you need to initialize your instance
bool SearchLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
	// 创建输入框
	inputEdit = EditBox::create(Size(visibleSize.width, INPUT_BOX_HEIGHT), "bk_input.png");
 	inputEdit->setFontSize(TEXT_INPUT_FONTSIZE);
 	inputEdit->setText("click here for input");
	inputEdit->setFontColor(Color4B(40, 180, 50, 255));
	inputEdit->setInputFlag(EditBox::InputFlag::SENSITIVE);
	inputEdit->setInputMode(EditBox::InputMode::SINGLE_LINE);
	inputEdit->setMaxLength(128);
	inputEdit->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - inputEdit->getContentSize().height/2 - 5));

	inputEdit->setDelegate(this);

	this->addChild(inputEdit, 1);

	Color3B backColor = Color3B(30,30,30);
	// 创建索引框
	_listView = ListView::create();
	// set list view ex direction
	_listView->setDirection(ScrollView::Direction::VERTICAL);
	_listView->setBounceEnabled(true);
	//_listView->setBackGroundColor(backColor, backColor);
	_listView->setBackGroundImage("bk_output.png");
	_listView->setBackGroundImageScale9Enabled(true);
	_listView->setContentSize(visibleSize);
	_listView->setPosition(Vec2(origin.x + (visibleSize.width - _listView->getContentSize().width) / 2,
		origin.y + visibleSize.height - inputEdit->getContentSize().height - _listView->getContentSize().height - 5));
	_listView->addEventListener((ListView::ccListViewCallback)CC_CALLBACK_2(SearchLayer::selectedItemEvent, this));
	//_listView->addEventListener((ListView::ccScrollViewCallback)CC_CALLBACK_2(UIListViewTest_Vertical::selectedItemEventScrollView, this));
	_listView->setScrollBarPositionFromCorner(Vec2(7, 7));
	// set all items layout gravity
	_listView->setGravity(ListView::Gravity::CENTER_VERTICAL);
	_listView->setVisible(false);
	addChild(_listView, 1);

	// set model
	default_item = MySearch::genDefaultItem();
	_listView->setItemModel(default_item);

    // 背景图片
    auto sprite = Sprite::create("bk_output.png");
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize / 2) + origin);
	sprite->setContentSize(visibleSize);
    // add the sprite as a child to this layer
	this->addChild(sprite);

	// 加载数据
	g_Dictionary.LoadData();

    return true;
}

void SearchLayer::update(float delta)
{
	Layer::update(delta);

	// 动态刷新listview
	updateListView();
}

void SearchLayer::editBoxEditingDidBegin(EditBox* editBox)
{
	log("editBox %p DidBegin !", editBox);
    if (nullptr != inputEdit)
    {
        inputEdit->setText("");
    }
	// 打开查找结果框
	if (nullptr != _listView)
	{
		_listView->setVisible(true);
	}
}

void SearchLayer::editBoxEditingDidEndWithAction(EditBox* editBox, EditBoxEndAction action)
{
	log("editBox %p DidEnd !", editBox);
	// 关闭查找结果框
	if (nullptr != _listView)
	{
//		_listView->setVisible(false);
// 		if (nullptr != inputEdit)
// 		{
// 			inputEdit->setText("click here for input");
// 		}
	}
}

void SearchLayer::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
	log("editBox %p TextChanged, text: %s ", editBox, text.c_str());
	// 更新查找结果框
	updateResultView();
}

void SearchLayer::editBoxReturn(EditBox* editBox)
{
	log("editBox %p was returned !", editBox);
}

void SearchLayer::selectedItemEvent(Ref *pSender, ListView::EventType type)
{
	switch (type)
	{
	case ListView::EventType::ON_SELECTED_ITEM_START:
	{
		ListView* listView = static_cast<ListView*>(pSender);
		auto item = listView->getItem(listView->getCurSelectedIndex());
		log("select child start index = %d", item->getTag());
		break;
	}
	case ListView::EventType::ON_SELECTED_ITEM_END:
	{
		ListView* listView = static_cast<ListView*>(pSender);
		auto item = listView->getItem(listView->getCurSelectedIndex());
		log("select child end index = %d", item->getTag());
		// 显示词语解释
		//selectView(item->getTag());
// 		void* idxdata = item->getUserData();
// 		string input = string((char*)idxdata);
		selectView(item->getName());

		break;
	}
	default:
		break;
	}
}

void SearchLayer::selectView(int selectId)
{
	ResultViewLayer* resultView = ResultViewLayer::create();
	if (nullptr != resultView)
	{
		resultView->updateResultView(selectId);

		addChild(resultView, 1);
	}
}

void SearchLayer::selectView(const string& input)
{
	ResultViewLayer* resultView = ResultViewLayer::create();
	if (nullptr != resultView)
	{
		resultView->updateResultView(input);

		addChild(resultView, 1);

        if (nullptr != inputEdit)
        {
            inputEdit->setVisible(false);
        }
		if (nullptr != _listView)
		{
			_listView->setVisible(false);
		}
	}
}

void SearchLayer::onResultViewClose(void)
{
	if (nullptr != inputEdit)
	{
		inputEdit->setVisible(true);
		inputEdit->setFocused(true);
	}
	if (nullptr != _listView)
	{
		_listView->setVisible(true);
	}
}

void SearchLayer::updateResultView()
{
	if (nullptr == _listView)
		return;

	// 清空缓存
	ResultCache.clear();
	// 移除掉所有子项
	_listView->removeAllItems();
	// 关闭心跳
	unscheduleUpdate();

	string input = inputEdit->getText();
	// 输入为空时 不查找
	if (input.empty())
	{
		return;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float width = visibleSize.width - 20;
	float textAreaWidth = width - 10;

	// 查找结果
	std::transform(input.begin(), input.end(), input.begin(), ::tolower);
	ResultCache = g_Dictionary.FindLikeAll(input);

	// 启动心跳
	if (ADD_ITEM_PERFRAME < ResultCache.size())
	{
		scheduleUpdate();
	}

	updateListView();
	
}

void SearchLayer::updateListView()
{
	if (0 == ResultCache.size())
	{
		unscheduleUpdate();
		return;
	}

	// 每次加载ADD_ITEM_PERFRAME个
	int cacheCount = ResultCache.size();
	auto loadCount = std::min(cacheCount, ADD_ITEM_PERFRAME);
	for (int i = 0; i < loadCount; ++i)
	{
		auto r = ResultCache[i];
		Widget* item = default_item->clone();
		item->setName(r.Index);
		Text* txt = (Text*)item->getChildByName("Title Text");
		if (txt)
		{
			string content = r.Index + ",";
			if (r.bEnglish)
			{
				content = content + r.Base.Chinese;
			}
			else
			{
				content = content + r.Base.English;
			}
			// 处理过长的内容
			//if (20 < content.length())
			//{
			//	content = content.substr(0, 20) + "...";
			//}
			// 设置内容
			txt->setString(content);
		}

		_listView->pushBackCustomItem(item);
	}

	// 移除加载项
	ResultCache.erase(ResultCache.begin(), ResultCache.begin() + loadCount);

	// set items margin
	float spacing = 4;
	_listView->setItemsMargin(spacing);

	//update listview data
	this->scheduleUpdate();

	_listView->forceDoLayout();
}
