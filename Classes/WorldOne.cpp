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
	auto deltaX = 0;
		auto deltaY = 0;
		auto city1 = Sprite::create("level_icon.png");

		

		for (auto i = 0; i < COUNT_LVLS_ON_1_WORLD; i++)
		{
			auto temp =  Sprite::create("level_icon.png");
			_levels.push_back(temp);
		}

	for (auto i = 0; i < COUNT_LVLS_ON_1_WORLD; i++)
	{
		
		
		_levels[i]->setPosition(Vec2(220 + deltaX, 600 + deltaY));
		addChild(_levels[i], 1);
		
		if ((i+1)%5 == 0)
		{
			deltaY -= 130;
			deltaX = 0;	
		}else
		{
			deltaX += 135;
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


	auto arrow = Sprite::create("down_arrow.png");
	arrow->setPosition(Vec2(215, 680));
	addChild(arrow, 1);


	// Create the actions
	auto moveDown = MoveBy::create(.7, Point(0, -15));
	auto moveUp = MoveBy::create(.7, Point(0, 15));

	arrow->runAction(RepeatForever::create(Sequence::create(moveDown, moveUp, NULL)));

	return true;
}