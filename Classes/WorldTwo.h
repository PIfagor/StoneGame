#ifndef _WORLD_TWO_H_
#define _WORLD_TWO_H_
#include "cocos2d.h"
#include "Enums.h"

class WorldTwo : public cocos2d::LayerColor
{
public:
	virtual ~WorldTwo();

	static cocos2d::Scene* createScene(bool inGame = false);

	virtual bool init() override;

	void update(float);

	CREATE_FUNC(WorldTwo);

private:
	WorldTwo();
	DISALLOW_COPY_AND_ASSIGN(WorldTwo);

	std::vector<cocos2d::Sprite*> _levels;
	cocos2d::EventListener*	_listener;

	int _deltaX = 135;
	int _deltaY = 130;

	cocos2d::Sprite* 	_arrow;

};



#endif 
