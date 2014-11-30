#ifndef _WORLD_ONE_H_
#define _WORLD_ONE_H_
#include "cocos2d.h"
#include "Enums.h"

class WorldOne : public cocos2d::LayerColor
{
public:
	virtual ~WorldOne();

	static cocos2d::Scene* createScene(bool inGame = false);

	virtual bool init() override;
	void update(float);


	CREATE_FUNC(WorldOne);

private:
	WorldOne();
	DISALLOW_COPY_AND_ASSIGN(WorldOne);

	std::vector<cocos2d::Sprite*> _levels;
	cocos2d::EventListener*	_listener;
	int _deltaX;
	int _deltaY;

	cocos2d::Sprite* 	_arrow;
};



#endif 
