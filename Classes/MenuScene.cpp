﻿#include "MenuScene.h"
#include "Applic.h"

#include "SimpleAudioEngine.h"

#include <cmath>
#include <sstream>


#include "json/document.h"
#include "json/prettywriter.h"
#include "json/filestream.h"


//#include "cocos-ext.h"


using namespace CocosDenshion;
//using namespace rapidjson;

// tiles bollean checkins
bool haveExit	=	false;
bool haveKey	=   false;
bool keyBounds  =	true;

Scene* MenuScene::createScene()
{
	Scene *	scene = Scene::create();
	MenuScene * layer = MenuScene::create();

	scene->addChild(layer);

	return scene;
}

#define PLAY_CRYSTAL SimpleAudioEngine::getInstance()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("pick_crystal.mp3").c_str());
#define WALK SimpleAudioEngine::getInstance()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("walk.mp3").c_str());
#define BGMUS SimpleAudioEngine::getInstance()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename("bg_music.mp3").c_str(),true);



bool MenuScene::init()
{
	
	if (!Layer::init())
	{
		return false;
	}

	_can_make_move=false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	TTFConfig config("arial.ttf", 30);//fonts/
	this->_pLabel = Label::createWithTTF(config, "Score 0");

	this->_pLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 + 400,
		origin.y + this->_pLabel->getContentSize().height));

	
	
	this->addChild(this->_pLabel, 1);

	auto wiew = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();


	//auto callback = std::bind(&MenuScene::menuReloadCallback, this, this);
	auto callback = std::bind(&MenuScene::show_menu, this, this);
	MenuItemImage *pCloseItem = MenuItemImage::create("cat.png","cat.png",callback);
		
	pCloseItem->setAnchorPoint(Vec2(0, 1));
	pCloseItem->setPosition(Vec2(0, wiew.height));

	Menu* pMenu = Menu::create(pCloseItem, NULL);
	pMenu->setPosition(Vec2(0, 0));
	this->addChild(pMenu, 2);
	// add "HelloWorld" splash screen"
	//Sprite* pSprite = Sprite::create("background.png");
	//// position the sprite on the center of the screen
	//pSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	//this->addChild(pSprite, 0);

	Sprite* pMarker = Sprite::create("marker.png");
	pMarker->setPosition(Vec2(0, 0));
	//this->addChild(pMarker, 0);

	this->schedule(schedule_selector(MenuScene::update));

	// IMPORTANT: enables touch events
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this, 0);

	_current_map = App::get()->getCurrentWorld() + App::get()->getCurrentMap();
	_grid = NULL;

	load_maps();


	FileUtils::getInstance()->addSearchPath("maps");
	//////////////////////////////////////////////////////////////


	
	std::stringstream ss;
	ss << "map_" << App::get()->getCurrentWorld() << "_" << App::get()->getCurrentMap()<<".json";
	change_level(ss.str());

	draw_controls();
	this->_controls_touch_id = 0;

	_score = 0;
	_is_running = true;

	 _menu_layer = MenuLayer::getMenu(Color4B(0, 0, 0, 200));
	this->addChild(_menu_layer, 10, Tags::MENU_TAG);
	_menu_layer->setVisible(false);

#ifdef _WIN32
	_player = new CXBOXController(1);
	_connected_controller = false;
#endif

	//SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename("cave.mp3").c_str(), true);

	//Director::getInstance()->getTouchDispatcher()->removeDelegate(this);
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this, 0);

	/*auto backBtn = MenuItemImage::create("back_midle.png", "back_midle.png", [](Ref*) {
		App::get()->gotoWorldMap(NULL);
	});
	
	backBtn->setAnchorPoint(Vec2(0, 1));
	backBtn->setPosition(Vec2(0, wiew.height));*/

	/*auto menu = Menu::create(backBtn, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 2);*/

	//AceCart method
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("animations/aceOfHeart.plist");
	//AceCart method
	SpriteBatchNode* spritebatch = SpriteBatchNode::create("animations/aceOfHeartAtlas.png");
	addChild(spritebatch);

	BGMUS
	addListeners();

	return true;
}

void MenuScene::update(float dt)
{
	// if is running

#ifdef _WIN32
	check_controller();
#endif
	if (this->_is_running) {
		if (
#ifdef _WIN32
            (
#endif
             _can_make_move
#ifdef _WIN32
			|| _connected_controller)
#endif
			&&this->_mv._can_move) {
			check_movement();

			check_level_end();
		}
	}

}

void MenuScene::check_level_end()
{
	if (this->_crystal_number == 0&&!haveExit) {
		App::get()->setConfig(true);
		App::get()->saveUserData();
		App::get()->setResult(WIN);
		handle_menu(NULL);
		//next_map();
	}
	App::get()->setResult(PAUSE);
}

void MenuScene::clear_map()
{
	_fallable.clear();
	this->_mv._is_moving = false;

	if (_grid != NULL) {
		for (int i = 0; i < ConfigVals::MAP_WIDTH * ConfigVals::MAP_HEIGHT; ++i)
		{
			this->removeChild(_grid[i].getSprite());

		}
		delete[] _grid;
		keyBounds = true;
	}
}

#ifdef _WIN32
void MenuScene::stop_vibration(float time)
{
	_player->Vibrate(0, 0);
}
#endif

void MenuScene::handle_menu(Ref* node)
{
	this->_is_running = !this->_is_running;
	if (this->_is_running) {
		//hide_menu();
	}
	else {
		show_menu(node);
	}
}

void MenuScene::show_menu(Ref* node)
{
	MenuLayer* menu = (MenuLayer*)this->getChildByTag(Tags::MENU_TAG);
	_menu_layer->setResults(App::get()->getResult());
	menu->setVisible(true);

	//Director::getInstance()->getTouchDispatcher()->removeDelegate(this);
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(menu, 0);
}

void MenuScene::hide_menu(Ref* node)
{
	MenuLayer* menu = (MenuLayer*)this->getChildByTag(Tags::MENU_TAG);
	menu->setVisible(false);

	//Director::getInstance()->getTouchDispatcher()->removeDelegate(menu);
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

#ifdef _WIN32
void MenuScene::check_controller()
{
	if (_player->IsConnected()) {
		Vec2 pt = _player->leftThumbTest();
		_connected_controller = true;


		if (_player->hasBtnBeenPressed(CXBOXController::BUTTON_BACK)) {
			Director::getInstance()->end();
		}

		if (_player->hasBtnBeenPressed(CXBOXController::BUTTON_B)) {
			App::get()->gotoWorldMap(NULL);
		}

		if (_player->hasBtnBeenPressed(CXBOXController::BUTTON_Y)) {
			menuReloadCallback(NULL);
		}

		if (_player->hasBtnBeenPressed(CXBOXController::BUTTON_START)) {
			handle_menu(NULL);
		}

		if (_player->hasBtnBeenPressed(CXBOXController::BUTTON_LB)) {
			prev_map();
		}

		if (_player->hasBtnBeenPressed(CXBOXController::BUTTON_RB)) {
			next_map();
		}


		handle_joystick(0, Vec2(MAP_WIDTH, 0), pt);
	}
	else {
		_connected_controller = false;
	}
}
#endif


void MenuScene::make_move()
{
	this->_mv._is_moving = true;
	_user->move(this, _mv._direction, callfuncN_selector(MenuScene::user_move_finished));
}

void MenuScene::check_movement()
{
	if (!this->_mv._is_moving) {

		int nextId = _user->getId();
		int oldId = _user->getId();
		bool has_moved = false;


		switch (_mv._direction)
		{
		case TOP:		nextId += ConfigVals::MAP_WIDTH;	break;
		case BOTTOM:	nextId -= ConfigVals::MAP_WIDTH;	break;
		case LEFT:		--nextId;							break;
		case RIGHT:		++nextId;							break;
		default:											break;
		}


		if (!this->isOnBounds(nextId))
		{
			if (_grid[nextId].getType() == SpriteType::STONE
				|| (_grid[nextId].getType() == SpriteType::GLASS

				)) {

				int next = nextId;

				float offset_x = ConfigVals::TILE_WIDTH;
				float offset_y = 0;
				float time = 0.2f;

				if (_mv._direction == LEFT) {
					--next;
					offset_x = -offset_x;
				}
				if (_mv._direction == RIGHT) {
					++next;
				}

				if (!_grid[next].getSprite()->isVisible()) {


					//// swap stone
					_grid[nextId].move(this, time, offset_x, offset_y, nullptr);
					std::swap(_grid[next], _grid[nextId]);

					_fallable.push_back(next);
				}
				else {
					if (_grid[nextId].getType() == SpriteType::GLASS)  {
						_grid[nextId].getSprite()->setVisible(false);
						_last_glass_id = nextId;
						this->scheduleOnce(schedule_selector(MenuScene::object_has_fallen), 0.3f);
					}
				}


			}
			else {

				if (_grid[nextId].getSprite()->isVisible()) {
					if (_grid[nextId].getType() == SpriteType::CRYSTAL) {
						_score += 50;
						update_score();
						PLAY_CRYSTAL;
						--(_crystal_number);
						//#ifdef _WIN32
						//						_player->Vibrate(ConfigVals::VIBRATION_POWER, ConfigVals::VIBRATION_POWER);
						//						this->scheduleOnce(schedule_selector(MenuScene::stop_vibration), 0.3);
						//#endif
					}

					if (_grid[nextId].getType() == SpriteType::SAND) {
						_score += 10;
						update_score();
					}
					
					if (_grid[nextId].getType() == SpriteType::EXIT) {
							haveExit = false;
					}

					if (_grid[nextId].getType() == SpriteType::KEY) {
						haveKey = false;
					}
					
				}


				//shitcode
				std::swap(_grid[nextId], _grid[oldId]);

				this->_user = &_grid[nextId];

				
				//WALK;

				if (_grid[oldId].getType() != SpriteType::GHOSTWALL) {
					
					if (_grid[oldId].getSprite() != nullptr && _grid[oldId].getSprite()->isVisible()) {
						_grid[oldId].getSprite()->setVisible(false);
					}
				}
				else
				{
					int i = 0;
					CCLOG("lol"+i);
				
				}
				


				_user->setId(nextId);
				make_move();

				has_moved = true;
			}

			if (has_moved) {
				_fallable.push_back(oldId + ConfigVals::MAP_WIDTH);
			}

		} // end if is on bounds
	} // end IF is moving
}

void MenuScene::update_score()
{
	std::stringstream ss;
	ss << "Score: " << _score;
	this->_pLabel->setString(ss.str().c_str());
}

void MenuScene::check_top_tiles(int user_pos)
{
	int free_cells = 0;
	int bottom_id = user_pos - ConfigVals::MAP_WIDTH;

	while (!(_grid[bottom_id].getSprite()->isVisible())) {
		++free_cells;
		bottom_id -= ConfigVals::MAP_WIDTH;
	}

	// LIST TILES THAT KILL USER
	if (_grid[bottom_id].getType() == SpriteType::USER
		&& (_grid[user_pos].getType() == SpriteType::STONE||_grid[user_pos].getType() == SpriteType::GLASS))
	{
		_mv._is_moving = true;
		CCLOG("USER HIT");
		App::get()->setResult(LOSE);
		
	}
	if (_grid[bottom_id].getType() == SpriteType::KEY
		&&	_grid[user_pos].getType() == SpriteType::STONE)
	{
		keyBounds = false;
		CCLOG("EXIT UNLOCK");
	}
	float offset_y = -free_cells * ConfigVals::TILE_HEIGHT;
	float offset_x = 0;
	float time = 0.2 * (float)free_cells;

#ifdef _WIN32
	float time_before_check = 0.2 * (float)(free_cells - 1);
#endif

	int top_id = user_pos;
	int under_top_id = user_pos - ConfigVals::MAP_WIDTH;
	while (_grid[top_id].getType() == SpriteType::CRYSTAL
		|| _grid[top_id].getType() == SpriteType::STONE
		|| _grid[top_id].getType() == SpriteType::GLASS
		|| _grid[top_id].getType() == SpriteType::NONE
		) {


		if (_grid[top_id].getSprite()->isVisible()) {
			auto callback = std::function<void()>();
			if (App::get()->getResult() == LOSE) {
				callback = [this] {
					CCLOG("You lost");
					show_menu(NULL);
				};
			}
				_grid[top_id].move(this, time, offset_x, offset_y, callback);
		}

		std::swap(_grid[top_id], _grid[top_id - free_cells * ConfigVals::MAP_WIDTH]);

		top_id += ConfigVals::MAP_WIDTH;
		under_top_id += ConfigVals::MAP_WIDTH;
	}
}

void MenuScene::user_move_finished(Node* sender)
{
	this->_mv._is_moving = false;

	for (auto it = _fallable.begin(); it != _fallable.end(); ++it) {
		int id = *(it);
		check_top_tiles(id);
	}

	_fallable.clear();
}

void MenuScene::object_has_fallen(float dt)
{
	_grid[_last_glass_id].setType(SpriteType::NONE);
	check_top_tiles(_last_glass_id + ConfigVals::MAP_WIDTH);
}

void MenuScene::draw_controls()
{
	/*_joy = Sprite::create("joystick_back.png");
	_stick = Sprite::create("joystick.png");
	this->addChild(_joy, 1);
	this->addChild(_stick, 1);

	hide_controls();*/
}

void MenuScene::show_controls(const Vec2 & pt)
{
	_joy->setPosition(pt);
	_stick->setPosition(pt);

	_joy->setVisible(true);
	_stick->setVisible(true);
}

void MenuScene::hide_controls()
{
	_joy->setVisible(false);
	_stick->setVisible(false);
}



void MenuScene::addListeners()
{
	auto listener = cocos2d::EventListenerTouchAllAtOnce::create();

	listener->onTouchesBegan = [&](const std::vector<Touch*>& pTouches, Event* event)
	{
		
		Touch* touch;

		Size DesResolution = Director::getInstance()->getWinSize();

		for (auto it = pTouches.begin(); it != pTouches.end(); it++) {

			touch = (Touch*)(*it);
			//last_id = touch->getID();
			Vec2 pt = touch->getLocation();
			_pointerJoistick=pt;
			
			this->_controls_touch_id = touch->getID();
		} // end for

	};

		listener->onTouchesMoved = [&](const std::vector<Touch*>& pTouches, Event* event){
			Touch*		touch;

			
			for (auto it = pTouches.begin(); it != pTouches.end(); it++) {
				touch = (Touch*)(*it);
				Vec2 pt = touch->getLocation();

				handle_joystick(touch->getID(), _pointerJoistick, pt);

			}
		};

		listener->onTouchesEnded = [&](const std::vector<Touch*>& pTouches, Event* event){
			Touch*		touch;


			for (auto it = pTouches.begin(); it != pTouches.end(); it++) {
				touch = (Touch*)(*it);
				Vec2 pt = touch->getLocation();

				if (touch->getID() == this->_controls_touch_id) {
					//hide_controls();
					//_pointerJoistick =  Vec2(0, 0);
					_can_make_move = false;
					this->_controls_touch_id = 0;
				}

			}
		};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);

		_listener = listener;
}


// NOTE: local, not visible outside this .cpp file
float angleBetweenLinesInRadians(Vec2 line1Start, Vec2 line1End)
{
	float rads = atan2(line1End.y - line1Start.y, line1End.x - line1Start.x);

	if (rads < 0) {
		rads += (2 * 3.14f);
	}

	return rads;
}

//NOTE: also local function
float radians_to_degrees(float radians)
{
	return radians * 57.29578;
}

void MenuScene::handle_joystick(int touch_id, const Vec2 & origin, const Vec2 & touch_pt)
{
	if (touch_id == this->_controls_touch_id)
	{
		/*std::stringstream ss;
		ss << "Controller x: " << touch_pt.x << ", y: " << touch_pt.y;
		CCLOG(ss.str().c_str());*/

		float back_x = origin.x; //_joy->getPositionX();
		float back_y = origin.y; //_joy->getPositionY();

		float touch_x = touch_pt.x;
		float touch_y = touch_pt.y;

		float delta_x = touch_x - back_x;
		float delta_y = touch_y - back_y;

		//float radius = _joy->getContentSize().width / 2 - 15;
		float delta = sqrt(pow((touch_x - back_x), 2) + pow(touch_y - back_y, 2));

		/*std::stringstream ss;
		ss <<"origin: (" << back_x << ", " << back_y << "), touch: (" << touch_x << ", y:" << touch_y << "), delta: " << delta;
		CCLOG(ss.str().c_str());*/

		/*if (delta <= radius) {
			this->_stick->setPosition(touch_pt);

		}
		else {
			float scale = delta / radius;
			float new_x = back_x + delta_x / scale;
			float new_y = back_y + delta_y / scale;

			this->_stick->setPosition(Vec2(new_x, new_y));
		}
*/
		if (delta > 50) {
			_can_make_move = true;
			this->_mv._can_move = true;
			// set_direction
			float alpha = radians_to_degrees(angleBetweenLinesInRadians(origin, touch_pt));
			Dir direction;


			/*std::stringstream ss;
			ss <<"ALPJA: " << alpha;
			CCLOG(ss.str().c_str());*/


			if ((alpha >= 315 && alpha <= 360) || (alpha >= 0 && alpha < 45)) {
				direction = RIGHT;
			}

			if (alpha >= 45 && alpha < 135) {
				direction = TOP;
			}

			if (alpha >= 135 && alpha < 225) {
				direction = LEFT;
			}

			if (alpha >= 225 && alpha < 315) {
				direction = BOTTOM;
			}


			this->_mv._direction = direction;
		}
		else {
			this->_mv._can_move = false;
		}

	}
}



void MenuScene::menuCloseCallback(Ref* pSender)
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

void MenuScene::menuReloadCallback(Ref* pSender)
{
	draw_grid(this->_map);
}

void MenuScene::draw_grid(std::string map)
{
	clear_map();
	haveExit = false;
	haveKey = false;
	_grid = new MapTile[ConfigVals::MAP_WIDTH * ConfigVals::MAP_HEIGHT];

	const std::string NONE = "NONE";
	const std::string SAND = "SAND";
	const std::string STONE = "STONE";
	const std::string CRYSTAL = "CRYSTAL";
	const std::string BORDER = "BORDER";
	const std::string USER = "USER";
	const std::string EMPTY = "EMPTY";
	const std::string GLASS = "GLASS";
	const std::string GHOSTWALL = "GHOSTWALL";
	const std::string EXIT = "EXIT";
	const std::string KEY = "KEY";
	////get relative path
	//std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(map.c_str());

	//// read map file content
	//unsigned long fileSize = 0;
	//unsigned char* pBuffer = FileUtils::getInstance()->getFileData(path.c_str(), "rb", &fileSize);

	//// convert map data to std::string
	//std::string content;
	//content.append((char*)(pBuffer));
	//content = content.substr(0, fileSize);

	std::string content = FileUtils::getInstance()->getStringFromFile(map);
	rapidjson::Document document;
	if (document.Parse<0>(content.c_str()).HasParseError()) {
		CCLOG("PARSE ERROR");
		//throw "Parse error";
	}

	Size DesResolution = Director::getInstance()->getWinSize();

	int offset_bottom = (DesResolution.height - ConfigVals::TILE_HEIGHT * 16) / 2;
	int offset_left = (DesResolution.width - ConfigVals::TILE_WIDTH * 20) / 2;

	int absolute_index(0);
	_crystal_number = 0;

	for (int i = 0; i < ConfigVals::MAP_HEIGHT; ++i)
	{
		for (int j = 0; j < ConfigVals::MAP_WIDTH; ++j)
		{
			int t = ConfigVals::MAP_HEIGHT - i - 1;
			int index = t * ConfigVals::MAP_WIDTH + j;

			std::string type = document[index]["type"].GetString();
			
			SpriteType tp = SpriteType::NONE;

			if (type != NONE) {
				Sprite* temp;

				std::string tex = document[index]["texture"].GetString();
				if (type == CRYSTAL){
					temp = AceCart();
				}
				
				else{
					temp = Sprite::create(tex.c_str());
				}

				if (type == SAND) {
					tp = SpriteType::SAND;
					//temp->setScale(0.5);
				}
				else

					if (type == EMPTY) {
						tp = SpriteType::SAND;
						temp->setVisible(false);
					}
					else

						if (type == BORDER) {
							tp = SpriteType::BORDER;
						}
						else

							if (type == CRYSTAL) {
								tp = SpriteType::CRYSTAL;
								++(_crystal_number);
							}
							else

								if (type == STONE) {
									tp = SpriteType::STONE;
								}
								else
									if (type == GHOSTWALL) {
										tp = SpriteType::GHOSTWALL;
									}
									else


										if (type == GLASS) {
											tp = SpriteType::GLASS;
										}
										else
											if (type == EXIT) {
												haveExit = true;
												tp = SpriteType::EXIT;
											}
											else
												if (type == KEY) {
													haveKey = true;
													temp->setScale(0.5);
													tp = SpriteType::KEY;
												}
												else

													if (type == USER) {
														this->_user = &_grid[absolute_index];
														tp = SpriteType::USER;
													}
													else {
														CCLOG( "Type doesn't exist.");
													}

													
													temp->setAnchorPoint(Vec2(0, 0));

													float x = j * ConfigVals::TILE_WIDTH;
													float y = i * ConfigVals::TILE_HEIGHT;

													temp->setPosition(Vec2(x + offset_left, y + offset_bottom));
													this->addChild(temp, Tags::TILE, Tags::TILE);

													if (type != BORDER) {
														auto lvlBack = Sprite::create("lvlBack.png");
														lvlBack->setAnchorPoint(Vec2(0, 0));
														lvlBack->setPosition(Vec2(x + offset_left, y + offset_bottom));
														this->addChild(lvlBack);
													}

													_grid[absolute_index] = MapTile(tp, temp, absolute_index);
																											
													if (tp == SpriteType::GLASS) {
														_grid[absolute_index].setBlocked(true);
													}

			}
			else {

				_grid[absolute_index] = MapTile(SpriteType::NONE, nullptr, absolute_index);
			}

			// add pointers to fallable objects to other vector to check physics
			/*if (tp == SpriteType::STONE || tp == SpriteType::CRYSTAL) {
			_fallable.push_back( &_grid[ absolute_index ]);
			}*/

			if (tp == SpriteType::USER) {
				//this->_user = &_grid[absolute_index];
				//tp = SpriteType::USER;
			}


			++absolute_index;

		}

	}

	//CCLOG("MAP LOADED");

}

void MenuScene::load_maps()
{
	/*std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("AAMapList.json");

	unsigned long fileSize = 0;
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rb", &fileSize);

	std::string content;
	content.append((char*)(pBuffer));
	content = content.substr(0, fileSize);*/


	std::string content = FileUtils::getInstance()->getStringFromFile("AAMapList.json");

	rapidjson::Document document;
	if (document.Parse<0>(content.c_str()).HasParseError()) {
		CCLOG("PARSE ERROR");
		//throw "Parse error";
	}


	for (size_t i = 0; i < document.Size(); ++i)
	{
		std::string elem = document[i].GetString() + std::string(".json");
		this->_map_vector.push_back(elem);
	}

}

void MenuScene::load_map(std::string s){

	assert(std::find(_map_vector.begin(), _map_vector.end(), s) != _map_vector.end());
	_map = s;

	menuReloadCallback(NULL);
		
}

void MenuScene::next_map()
{
	if (_current_map + 1 < _map_vector.size()) {
		++(_current_map);
		change_level("");
	}
}

void MenuScene::prev_map()
{
	if (_current_map - 1 >= 0) {
		--(_current_map);
		change_level("");
	}
}

void MenuScene::change_level(std::string carMap)
{
	if (carMap.empty()){
		if (_current_map >= 0 && _current_map < _map_vector.size()) {
			_map = _map_vector.at(_current_map);

			menuReloadCallback(NULL);
		}
	}
	else {
		_map = carMap;
		menuReloadCallback(NULL);
	}
	

}

bool MenuScene::isOnBounds(int index)
{
	if (_grid[index].getType() == SpriteType::BORDER) {
		return true;
	}

	if (_grid[index].getType() == SpriteType::EXIT){
		
		if (this->_crystal_number == 0) {
			if (haveKey == false) {
				return false;
			}
			else return true;
		}
		return true;
	}

	if (_grid[index].getType() == SpriteType::KEY){
		return keyBounds;
	}
	return false;
}

Sprite* MenuScene::AceCart()
{
	
	Vector<SpriteFrame*> animFrames(25);
	char str[100] = { 0 };
	for (int i = 1; i <26; i++)
	{
		sprintf(str, "%d.png", i);
		//auto frame = SpriteFrame::create(str, Rect(0, 0, 45, 45)); //we assume that the sprites' dimentions are 40*40 rectangles.
		auto frame = SpriteFrameCache::getInstance()->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.12f);
	auto sprite2 = Sprite::createWithSpriteFrameName("1.png");
	sprite2->setPosition(Vec2(100, 100));


	auto action = RepeatForever::create(Animate::create(animation));
	sprite2->runAction(action);

	return sprite2;
}

MenuScene::~MenuScene()
{
	delete[] _grid;
#ifdef _WIN32
	delete _player;
#endif
	Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
}
