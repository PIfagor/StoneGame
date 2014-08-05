#ifndef __SLPASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"

class Splash : public cocos2d::LayerColor
{
public:
	virtual ~Splash();

	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(Splash);

private:
	Splash();
	DISALLOW_COPY_AND_ASSIGN(Splash);
};

#endif 
