#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "search_cocos/SearchScene.h"

USING_NS_CC;
using namespace MySearch;

Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

//     // add a "close" icon to exit the progress. it's an autorelease object
//     auto closeItem = MenuItemImage::create(
//                                         "CloseNormal.png",
//                                         "CloseSelected.png",
//                                         CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
//     
//     closeItem->setPosition(origin + Vec2(visibleSize) - Vec2(closeItem->getContentSize() / 2));
// 
//     // create menu, it's an autorelease object
//     auto menu = Menu::create(closeItem, nullptr);
//     menu->setPosition(Vec2::ZERO);
//     this->addChild(menu, 1);

	// ²éÕÒ½çÃæ
	auto SearchScene = SearchLayer::create();
	SearchScene->setPosition(0, 0);
	this->addChild(SearchScene, 1);

    return true;
}

void HelloWorld::menuCloseCallback(Ref* sender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
