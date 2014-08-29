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

#ifdef _WIN32
	//install controler buttons
	App::get()->_player->clear_controller();
	App::get()->_player->setButtomFunction([]{ Director::getInstance()->end(); }, CXBOXController::BUTTON_BACK);
	App::get()->_player->setButtomFunction([this]{ App::get()->gotoWorldMap(NULL); },
		CXBOXController::BUTTON_START);
	App::get()->_player->setButtomFunction([this]{App::get()->gotoWorldMap(NULL); },
		CXBOXController::BUTTON_A);
	
#endif
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
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("animations/aceOfHeart.plist");

	SpriteBatchNode* spritebatch = SpriteBatchNode::create("animations/aceOfHeartAtlas.png");
	addChild(spritebatch);


	Vector<SpriteFrame*> animFrames(25);
	char str[100] = { 0 };
	for (int i = 1; i <26; i++)
	{
		sprintf(str, "%d.png", i);
		//auto frame = SpriteFrame::create(str, Rect(0, 0, 45, 45)); //we assume that the sprites' dimentions are 40*40 rectangles.
		auto frame = SpriteFrameCache::getInstance()->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.08f);
	auto sprite2 = Sprite::createWithSpriteFrameName("1.png");
	sprite2->setPosition(Vec2(100, 100));
	sprite2->setAnchorPoint(Vec2(1, 0));
	sprite2->setPosition(Vec2(desSize.width, 0));

	auto action = RepeatForever::create(Animate::create(animation));
	sprite2->runAction(action);
	addChild(sprite2, 2);

	auto menu = Menu::create(startGame, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,1);
	
	this->schedule(schedule_selector(MainMenu::update));
	



	return true;

	}

void MainMenu::update(float dt)
{
	// if is running
	//CCLOG("check_1up");
#ifdef _WIN32
	App::get()->_player->check_controller();
#endif	

}
