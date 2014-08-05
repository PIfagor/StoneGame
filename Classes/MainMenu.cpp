#include "MainMenu.h"
#include "Applic.h"


USING_NS_CC;

MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
}

Scene* MainMenu::createScene()
{
	auto scene = Scene::create();
	auto layer = MainMenu::create();
	scene->addChild(layer);

	return scene;
}

bool MainMenu::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
	{
		return false;
	}

	/*auto sprite = Sprite3D::create("pyr.obj", "texture.tga");
	sprite->setScale(0.3);
	sprite->setRotation3D(Vec3(0, 230, 0));
	sprite->setPosition(Vec2(730, 130));
	this->addChild(sprite);

	ActionInterval* action = RotateBy::create(5, Vec3(0, 360, 0));
	auto seq = Sequence::create(action, NULL);
	sprite->runAction(RepeatForever::create(seq));
	*/

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto desSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

	auto sprite = Sprite::create("kossaks.png");
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(sprite, 0);

	auto startCallback = std::bind(&App::gotoWorldMap, App::get(), this);
	auto startGame = MenuItemImage::create("play.png", "play.png", startCallback);
	startGame->setAnchorPoint(Vec2(1, 0));
	startGame->setPosition(Vec2(desSize.width, 0));
	//addChild(startGame, 1);

	/*auto resumeCallback = std::bind(&App::resumeGame, App::get(), this);
	auto resumeGame = MenuItemImage::create("resume.png", "resume.png", resumeCallback);
	resumeGame->setPosition(Vec2(200, 500));*/

	auto menu = Menu::create(startGame, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,1);
	

	return true;
}

