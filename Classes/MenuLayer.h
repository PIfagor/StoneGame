#ifndef _MENU_LAYER_H_
#define _MENU_LAYER_H_

#include "cocos2d.h"

USING_NS_CC;

class MenuLayer : public LayerColor {
public:

	static MenuLayer* getMenu(cocos2d::Color4B);
	virtual bool init();

	void menuCloseCallback(Ref*);

	CREATE_FUNC(MenuLayer);

};


#endif