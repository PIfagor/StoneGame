#include "WorldOne.h"


#include "Applic.h"

USING_NS_CC;

WorldOne::WorldOne()
{
}

WorldOne::~WorldOne()
{

	if (_listener) {
		Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
	}
}

Scene* WorldOne::createScene(bool inGame)
{
	auto scene = Scene::create();
	auto layer = WorldOne::create();
	scene->addChild(layer);

	
	return scene;
}



bool WorldOne::init()
{


	App::get()->setCurrentWorld (0);
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
	{
		return false;
	}

#ifdef _WIN32
		//install controler buttons
		App::get()->_player->clear_controller();
		App::get()->_player->setButtomFunction([]{ Director::getInstance()->end(); }, CXBOXController::BUTTON_BACK);
		App::get()->_player->setButtomFunction([this]
		{ if (App::get()->getCurrentMap() <= 0)
		App::get()->setCurrentMap(COUNT_LVLS_ON_1_WORLD - 1);
		else
			App::get()->decrementCurrnetMap();
		_arrow->setPosition(Vec2(215 + _deltaX * (App::get()->getCurrentMap() % 5), 680 - _deltaY * (App::get()->getCurrentMap() / 5))); },
			CXBOXController::BUTTON_LEFT);
		App::get()->_player->setButtomFunction([this]
		{if (App::get()->getCurrentMap() >= COUNT_LVLS_ON_1_WORLD - 1)
		App::get()->setCurrentMap(0);
		else
			App::get()->incrementCurrnetMap();
		_arrow->setPosition(Vec2(215 + _deltaX * (App::get()->getCurrentMap() % 5), 680 - _deltaY * (App::get()->getCurrentMap() / 5)));
		},
			CXBOXController::BUTTON_RIGHT);
		App::get()->_player->setButtomFunction([this]{ App::get()->startLevel(NULL); },
			CXBOXController::BUTTON_START);
		App::get()->_player->setButtomFunction([this]{ App::get()->startLevel(NULL); },
			CXBOXController::BUTTON_A);
		App::get()->_player->setButtomFunction([this]{ App::get()->gotoWorldMap(NULL); },
			CXBOXController::BUTTON_B);
#endif


	auto backBtn = MenuItemImage::create("back.png", "back.png", [](Ref*) {
		App::get()->gotoWorldMap(NULL);
	});
	auto wiew = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	backBtn->setAnchorPoint(Vec2(0,1));
	backBtn->setPosition(Vec2(0,wiew.height ));

	auto menu = Menu::create(backBtn, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 2);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sprite = Sprite::create("papyrus.png");
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(sprite, 0);
	
			

		for (auto i = 0; i < COUNT_LVLS_ON_1_WORLD; i++)
		{
			Sprite* temp;
			if (App::get()->getConfig()._complitedLvls[0][i] == false)
				temp = Sprite::create("level_icon.png");
			else 
				temp = Sprite::create("level_icon_cmpl.png");

			_levels.push_back(temp);
		}
		auto deltaX = 0;
		auto deltaY = 0;
	for (auto i = 0; i < COUNT_LVLS_ON_1_WORLD; i++)
	{
		
		
		_levels[i]->setPosition(Vec2(220 + deltaX, 600 + deltaY));
		addChild(_levels[i], 1);
		
		if ((i+1)%5 == 0)
		{
			deltaY -= _deltaY;
			deltaX = 0;	
		}else
		{
			deltaX += _deltaX;
		}
		
	}
	

	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	_listener = listener;


	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = touch->getLocation();

		for (auto i = 0; i < COUNT_LVLS_ON_1_WORLD; i++)
		{
			if (_levels[i]->getBoundingBox().containsPoint(p) && App::get()->canTouch())
			{
				App::get()->setCurrentMap(i);
				App::get()->startLevel(NULL);
				return true;
			}
		}
		

		return false;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);

#ifdef _WIN32
	_arrow = Sprite::create("down_arrow.png");
	_arrow->setPosition(Vec2(215 + _deltaX * (App::get()->getCurrentMap() % 5), 680 - _deltaY * (App::get()->getCurrentMap() / 5)));
	addChild(_arrow, 1);
	// Create the actions
	auto moveDown = MoveBy::create(.7, Point(0, -15));
	auto moveUp = MoveBy::create(.7, Point(0, 15));
	_arrow->runAction(RepeatForever::create(Sequence::create(moveDown, moveUp, NULL)));
#endif

	this->schedule(schedule_selector(WorldOne::update));

	return true;
}



void WorldOne::update(float dt)
{
	// if is running
	//CCLOG("check_1up");
#ifdef _WIN32
	App::get()->_player->check_controller();
#endif	

}
