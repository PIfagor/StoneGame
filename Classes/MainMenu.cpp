#include "MainMenu.h"
#include "Applic.h"

#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

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

#define PLAY_THUNDER //SimpleAudioEngine::getInstance()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("thunder_2.mp3").c_str());
#define BGMUS //SimpleAudioEngine::getInstance()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename("rain.mp3").c_str(),true);


void MainMenu::playLights()
{
	int dt = rand() % 5 + 10;

	CCLOG("dt: %d", dt);
	
	auto seq1 = Sequence::create(DelayTime::create(dt), CallFunc::create([this] {
		//AudioManager::get()->playSound("ravens.mp3");
		//SimpleAudioEngine::getInstance()->playEffect("ravens.mp3");

		light1->setOpacity(255);
		auto fadeTo		= FadeTo::create(0.5, 0);
		PLAY_THUNDER;
		light1->runAction(Sequence::create(fadeTo, CallFunc::create([this] {
			
			light2->setOpacity(255);
			auto fadeTo2	= FadeTo::create(1, 0);
			light2->runAction(Sequence::create(fadeTo2, nullptr));
		}), nullptr));

		playLights();
	}), nullptr);
	this->runAction(seq1);
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
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto desSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

	auto sprite = Sprite::create("alice_main.png");
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(sprite, 0);

	auto alice_overlay = Sprite::create("main/alice_overlay.png");
	alice_overlay->setPosition(Vec2(512, 464));
	addChild(alice_overlay, 1);

	auto fadeTo		= FadeTo::create(1, 100);
	auto fadeFrom	= FadeTo::create(0.9, 255);
	alice_overlay->runAction( RepeatForever::create(Sequence::create(fadeTo, fadeFrom, NULL)) );

	auto glass_layer = Sprite::create("main/glass_layer.png");
	glass_layer->setPosition(Vec2(511, 448));
	glass_layer->setOpacity(255*0.7);
	addChild(glass_layer, 2);

	light1 = Sprite::create("main/lightning_overlay_1.png");
	light1->setPosition(513, 463);
	addChild(light1, 2);
	light1->setOpacity(0);
	playLights();

	light2 = Sprite::create("main/lightning_overlay_2.png");
	light2->setPosition(513, 463);
	addChild(light2, 2);
	light2->setOpacity(0);


	/////////////////////////////
	auto clipper = ClippingNode::create();
    clipper->setContentSize(  Size(227, 363) );
	clipper->setAnchorPoint(Vec2(0.5, 0.5));
    clipper->setPosition( Vec2(514, 464) );
    addChild(clipper, 1);

    auto stencil = DrawNode::create();
    Vec2 figure[8];
	figure[0] = Vec2(46, 12);
	figure[1] = Vec2(14, 45);
	figure[2] = Vec2(14, 310);
	figure[3] = Vec2(60, 354);
	figure[4] = Vec2(170, 354);
	figure[5] = Vec2(217, 310);
	figure[6] = Vec2(217, 45);
	figure[7] = Vec2(170, 12);

    Color4F white(1, 1, 1, 1);
	stencil->drawPolygon(figure, 8, white, 1, white);
    clipper->setStencil(stencil);

	auto content = ParticleRain::create();
	content->setEmissionRate(300.);
	content->setLife(2.5);
	content->setSpeed(150);
	//content->setScale(2.);
	content->setGravity(Vec2(100, -200));

	//auto content = ParticleSystemQuad::create("Snowing2.plist");
	content->setPosition( Vec2(clipper->getContentSize().width/2 - 100, clipper->getContentSize().height + 10) );
	content->setAnchorPoint(Vec2(0.5, 1));
    clipper->addChild(content);

	//////////////////////////////


	auto moon_overlay = Sprite::create("main/moon_overlay.png");
	moon_overlay->setPosition(564, 630);
	addChild(moon_overlay, 1);
	moon_overlay->runAction( RepeatForever::create(Sequence::create(FadeTo::create(1.5, 0), FadeTo::create(1.3, 255), NULL)) );

	auto sky1 = Sprite::create("main/sky_overlay_1.png");
	sky1->setPosition(511, 555);
	addChild(sky1, 1);
	sky1->runAction( RepeatForever::create(Sequence::create(FadeTo::create(1.5, 0), FadeTo::create(1.3, 255), NULL)) );

	auto sky2 = Sprite::create("main/sky_overlay_2.png");
	sky2->setPosition(510, 559);
	addChild(sky2, 1);
	sky2->runAction( RepeatForever::create(Sequence::create(FadeTo::create(1.7, 255), FadeTo::create(1.5, 0), NULL)) );


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
	
	BGMUS;


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
