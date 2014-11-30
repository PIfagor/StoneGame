#include "WorldMap.h"


#include "Applic.h"

USING_NS_CC;

WorldMap::WorldMap()
{
	deltaX = 260;
	deltaY = 175;
}

WorldMap::~WorldMap()
{

	if (_listener) {
		Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
	}
}

Scene* WorldMap::createScene(bool inGame)
{
	auto scene = Scene::create();
	auto layer = WorldMap::create();
	scene->addChild(layer);


	return scene;
}


bool WorldMap::init()
{

	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
	{
		return false;
	}
#ifdef _WIN32
	//install controler buttons
	App::get()->_player->clear_controller();
	App::get()->_player->setButtomFunction([]{ App::get()->gotoMenu(NULL); }, CXBOXController::BUTTON_BACK);
	App::get()->_player->setButtomFunction([]{ App::get()->gotoMenu(NULL); }, CXBOXController::BUTTON_B);
	App::get()->_player->setButtomFunction([this]
	{ if (App::get()->getCurrentWorld() <= 0)
	App::get()->setCurrentWorld(COUNT_WORLDS - 1);
	else
		App::get()->decrementCurrnetWorld();
	_arrow->setPosition(Vec2(215 + deltaX*App::get()->getCurrentWorld(), 680 - deltaY*App::get()->getCurrentWorld())); },
		CXBOXController::BUTTON_LEFT);
	App::get()->_player->setButtomFunction([this]
	{if (App::get()->getCurrentWorld() >= COUNT_WORLDS - 1)
	App::get()->setCurrentWorld(0);
	else
		App::get()->incrementCurrnetWorld();
	_arrow->setPosition(Vec2(215 + deltaX*App::get()->getCurrentWorld(), 680 - deltaY*App::get()->getCurrentWorld())); },
		CXBOXController::BUTTON_RIGHT);
	App::get()->_player->setButtomFunction([this]{ openActiveWorld(App::get()->getCurrentWorld()); },
		CXBOXController::BUTTON_START);
	App::get()->_player->setButtomFunction([this]{ openActiveWorld(App::get()->getCurrentWorld()); },
		CXBOXController::BUTTON_A);
#endif


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sprite = Sprite::create("papyrus.png");
	assert(sprite != NULL);
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(sprite, 0);

	

	auto city1 = Sprite::create("potlava.png");
	city1->setPosition(Vec2(215 + deltaX * 0, 560 - deltaY * 0));
	addChild(city1, 1);

	auto city2 = Sprite::create("globuno.png");
	city2->setPosition(Vec2(215 + deltaX * 1, 560 - deltaY * 1));
	addChild(city2, 1);

	auto city3 = Sprite::create("lusenko.png");
	city3->setPosition(Vec2(215 + deltaX * 2, 560 - deltaY * 2));
	addChild(city3, 1);

	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	_listener = listener;
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [city1, city2, city3, this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = touch->getLocation();
		if (city1->getBoundingBox().containsPoint(p) && App::get()->canTouch())
		{
			//App::get()->gotoWorldOne(NULL);
			openActiveWorld(0);
			return true;
		}
		if (city2->getBoundingBox().containsPoint(p) && App::get()->canTouch())
		{

			//App::get()->gotoWorldTwo(NULL);
			openActiveWorld(1);
			return true;
		}
		if (city3->getBoundingBox().containsPoint(p) && App::get()->canTouch())
		{
			//App::get()->gotoWorldThree(NULL);
			openActiveWorld(2);
			return true;
		}
		return false;
	};


	this->schedule(schedule_selector(WorldMap::update));

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);

#ifdef _WIN32
	 _arrow = Sprite::create("down_arrow.png");
	 _arrow->setPosition(Vec2(215 + deltaX*App::get()->getCurrentWorld(), 680 - deltaY*App::get()->getCurrentWorld()));
	 addChild(_arrow, 1);
	// Create the actions
	auto moveDown = MoveBy::create(.7, Point(0, -15));
	auto moveUp = MoveBy::create(.7, Point(0, 15));
	_arrow->runAction(RepeatForever::create(Sequence::create(moveDown, moveUp, NULL)));
#endif
	return true;
}

void WorldMap::openActiveWorld(int number)
{
	switch (number)
	{
	case 0:
		App::get()->gotoWorldOne(NULL);
		break;
	case 1:
		App::get()->gotoWorldTwo(NULL);
		break;
	case 2:
		App::get()->gotoWorldThree(NULL);
		break;
	default:
		CCLOG("This world is not exitst");
		break;
	}
	return;
};


void WorldMap::update(float dt)
{
	// if is running
	//CCLOG("check_1up");
#ifdef _WIN32
	App::get()->_player->check_controller();
#endif	
	
}



