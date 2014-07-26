#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Enums.h"

#include "MenuLayer.h"

#include <vector>
#include "MapTile.h"

#ifdef _WIN32   
#include "XBOXController.h"
#endif

USING_NS_CC;

class MenuScene : public Layer
{
public:

	virtual bool init();
	static cocos2d::Scene* createScene();

	void menuCloseCallback(Ref*);
	void menuReloadCallback(Ref*);

	//THE OVERRIDES
	//virtual void ccTouchesBegan(CCSet*, CCEvent*);
	//virtual void ccTouchesMoved(CCSet*, CCEvent*);
	//virtual void ccTouchesEnded(CCSet*, CCEvent*);

	void update(float);
	// implement the "static node()" method manually
	CREATE_FUNC(MenuScene);

	~MenuScene();

private:

	void draw_grid(std::string);
	void draw_controls();
	void show_controls(const Vec2 &);
	void hide_controls();
	void handle_joystick(int, const Vec2 &, const Vec2 &);
	void check_movement();
	bool isOnBounds(int);
	void user_move_finished(Node*);
	void object_has_fallen(float);
	void make_move();
	void check_top_tiles(int);
	void update_score();
	void show_menu();
	void hide_menu();
	void handle_menu();
	void check_level_end();
	void clear_map();

	void next_map();
	void prev_map();
	void change_level();

	void load_maps();


	struct move_controller {
		Dir		_direction;
		bool	_is_moving;
		bool	_can_move;

		move_controller() :
			_direction(TOP),
			_is_moving(false),
			_can_move(false)
		{
			// struct constructor body
		}
	};


	// vars
	MapTile*			_grid;
	move_controller		_mv;
	Label*			_pLabel;

	Sprite*					_joy;
	Sprite*					_stick;
	int							_controls_touch_id;
	MapTile*					_user;
	std::vector<int>			_fallable;
	int _score;
	bool						_is_running;
	MenuLayer*					_menu_layer;

	std::string					_map;
	int							_crystal_number;

	std::vector<std::string>	_map_vector;
	size_t							_current_map;

	int							_last_glass_id;

#ifdef _WIN32
	CXBOXController *	_player;
	bool				_connected_controller;

	void check_controller();
	void stop_vibration(float);

#endif

};

#endif // __HELLOWORLD_SCENE_H__
