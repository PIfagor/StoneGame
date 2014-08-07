#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::LayerColor
{
public:
	virtual ~MainMenu();
	static cocos2d::Scene* createScene();

	virtual bool init();
	void update(float);
	CREATE_FUNC(MainMenu);

private:
	MainMenu();
	DISALLOW_COPY_AND_ASSIGN(MainMenu);
};

#endif 
