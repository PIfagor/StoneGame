#ifndef _WORLD_THREE_H_
#define _WORLD_THREE_H_
#include "cocos2d.h"
#include "Enums.h"

class WorldThree : public cocos2d::LayerColor
{
public:
	virtual ~WorldThree();

	static cocos2d::Scene* createScene(bool inGame = false);

	virtual bool init() override;

	void update(float);

	CREATE_FUNC(WorldThree);

private:
	WorldThree();
	DISALLOW_COPY_AND_ASSIGN(WorldThree);

	std::vector<cocos2d::Sprite*> _levels;
	cocos2d::EventListener*	_listener;

	int _deltaX;
	int _deltaY;

	cocos2d::Sprite* 	_arrow;

};
#endif 
