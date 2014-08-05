#include "WorldMap.h"


#include "Applic.h"

USING_NS_CC;

WorldMap::WorldMap() :
_inGameMode(false)
{
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

	if (inGame) {
		layer->setIngameMode();
	}

	return scene;
}

void WorldMap::setIngameMode() {
	_inGameMode = true;

	auto backBtn = MenuItemImage::create("back.png", "back.png", [](Ref*) {
		auto director = Director::getInstance();
		director->popScene();
		CCLOG("POPPING world map");
	});

	Size visibleSize = Director::getInstance()->getVisibleSize();
	backBtn->setPosition(Vec2(visibleSize.width / 2, 140));

	auto menu = Menu::create(backBtn, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);
};

bool WorldMap::init()
{

	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sprite = Sprite::create("papyrus.png");
	assert(sprite != NULL);
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(sprite, 0);

	auto deltaX = 260;
	auto deltaY = 175;

	auto city1 = Sprite::create("potlava.png");
	city1->setPosition(Vec2(215 + deltaX*0, 560 - deltaY*0));
	addChild(city1, 1);
	
	auto city2 = Sprite::create("globuno.png");
	city2->setPosition(Vec2(215 + deltaX*1, 560 - deltaY*1));
	addChild(city2, 1);

	auto city3 = Sprite::create("lusenko.png");
	city3->setPosition(Vec2(215 + deltaX*2, 560 - deltaY*2));
	addChild(city3, 1);

	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	_listener = listener;
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [city1, city2, city3, this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = touch->getLocation();
		if (city1->getBoundingBox().containsPoint(p) && App::get()->canTouch())
		{
			App::get()->gotoWorldOne(NULL);
			return true;
		}
		if (city2->getBoundingBox().containsPoint(p) && App::get()->canTouch())
		{
			App::get()->gotoWorldTwo(NULL);
			return true;
		}
		if (city3->getBoundingBox().containsPoint(p) && App::get()->canTouch())
		{
			App::get()->gotoWorldThree(NULL);
			return true;
		}
		return false;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);


	auto arrow = Sprite::create("down_arrow.png");
	arrow->setPosition(Vec2(215, 680));
	addChild(arrow, 1);


	// Create the actions
	auto moveDown = MoveBy::create(.7, Point(0, -15));
	auto moveUp = MoveBy::create(.7, Point(0, 15));

	arrow->runAction(RepeatForever::create(Sequence::create(moveDown, moveUp, NULL)));

	return true;
}