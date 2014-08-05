#ifndef _WORLD_MAP_H_
#define _WORLD_MAP_H_

#include "cocos2d.h"
#include "Enums.h"

class WorldMap : public cocos2d::LayerColor
{
public:
	virtual ~WorldMap();

	static cocos2d::Scene* createScene(bool inGame = false);

	virtual bool init() override;
	void setIngameMode();


	CREATE_FUNC(WorldMap);

private:
	WorldMap();
	DISALLOW_COPY_AND_ASSIGN(WorldMap);

	cocos2d::EventListener*	_listener;


	bool	_inGameMode;
};

#endif 
