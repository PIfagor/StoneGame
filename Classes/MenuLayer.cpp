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
		
	
	this->setResults(App::get()->getResult());
	return true;
}

void MenuLayer::setResults(Results r)
{
	auto wiew = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	auto callback = std::bind(&MenuLayer::menuCloseCallback, this, this);
	MenuItemImage* goToLevels;
	MenuItemImage* nextReload;
	auto goNextMap = std::bind(&MenuLayer::nextMap, this, this);
	auto reload = std::bind(&MenuLayer::reloadMap, this, this);
	auto back = std::bind(&MenuLayer::backToMap, this, this);
	switch (r)
	{
	case WIN:

		nextReload = MenuItemImage::create("exit_pressed.png", "exit_pressed.png", goNextMap);
		break;
	case LOSE:

		nextReload = MenuItemImage::create("exit_pressed.png", "exitGame.png", reload);
		break;
	case PAUSE:
		nextReload = MenuItemImage::create("exitGame.png", "exit_pressed.png", back);
		break;
	default:
		break;
	}
		goToLevels = MenuItemImage::create("exitGame.png", "exit_pressed.png", callback);
		goToLevels->setAnchorPoint(Vec2(1, 0));
		goToLevels->setPosition(Vec2(wiew.width, 0));

		nextReload->setAnchorPoint(Vec2(1, 0));
		nextReload->setPosition(Vec2(wiew.width, nextReload->getBoundingBox().size.height));

		Menu* pMenu = Menu::create(goToLevels, nextReload,  NULL);
		pMenu->setPosition(Point::ZERO);
		addChild(pMenu, 1);
	
};

void MenuLayer::reloadMap(Ref *node)
{
	App::get()->startLevel(NULL);
};
void MenuLayer::nextMap(Ref *node)
{
	App::get()->incrementCurrnetMap();
	App::get()->startLevel(NULL);
};
void MenuLayer::backToMap(Ref *node)
{

};


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