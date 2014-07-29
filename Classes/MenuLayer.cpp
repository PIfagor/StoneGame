#include "MenuLayer.h"


MenuLayer* MenuLayer::getMenu(cocos2d::Color4B var)
{
	MenuLayer* layer = MenuLayer::create();
	layer->initWithColor(var);

	return layer;
}

bool MenuLayer::init()
{
	if (!LayerColor::init())
	{
		return false;
	}


	auto callback = std::bind(&MenuLayer::menuCloseCallback, this, this);
	MenuItemImage *pCloseItem = MenuItemImage::create(
		"exitGame.png",
		"exit_pressed.png",
		callback);

	Menu* pMenu = Menu::create(pCloseItem, NULL);
	pMenu->setPosition(Vec2(500, 500));
	this->addChild(pMenu, 1);

	return true;
}


void MenuLayer::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}