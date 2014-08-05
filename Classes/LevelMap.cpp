
#include "LevelMap.h"
#include "Applic.h"

USING_NS_CC;

LevelMap::LevelMap() :
_inGameMode(false)
{
}

LevelMap::~LevelMap()
{
	if (_listener) {
		Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
	}
}

Scene* LevelMap::createScene(bool inGame)
{
	auto scene = Scene::create();
	auto layer = LevelMap::create();
	scene->addChild(layer);

	if (inGame) {
		layer->setIngameMode();
	}

	return scene;
}

void LevelMap::setIngameMode() {
	_inGameMode = true;

	auto backBtn = MenuItemImage::create("map_back.png", "map_back.png", [](Ref*) {
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

bool LevelMap::init()
{

	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sprite = Sprite::create("map.png");
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(sprite, 0);

	auto city1 = Sprite::create("map_city_1.png");
	city1->setPosition(Vec2(833, 560));
	addChild(city1, 1);

	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	_listener = listener;

	listener->setSwallowTouches(true);

	listener->onTouchBegan = [city1, this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = touch->getLocation();

		if (city1->getBoundingBox().containsPoint(p) && App::get()->canTouch())
		{
			if (!_inGameMode) {
				//App::get()->startGame(NULL);
			}

			return true;
		}

		return false;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);


	auto arrow = Sprite::create("down_arrow.png");
	arrow->setPosition(Vec2(833, 680));
	addChild(arrow, 1);


	// Create the actions
	auto moveDown = MoveBy::create(.7, Point(0, -15));
	auto moveUp = MoveBy::create(.7, Point(0, 15));

	arrow->runAction(RepeatForever::create(Sequence::create(moveDown, moveUp, NULL)));

	return true;
}