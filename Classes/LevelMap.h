#ifndef _LEVEL_MAP_H_
#define _LEVEL_MAP_H_

#include "cocos2d.h"

class LevelMap : public cocos2d::LayerColor
{
public:
	virtual ~LevelMap();

	static cocos2d::Scene* createScene(bool inGame = false);

	virtual bool init() override;
	void setIngameMode();


	CREATE_FUNC(LevelMap);

private:
	LevelMap();
	DISALLOW_COPY_AND_ASSIGN(LevelMap);

	cocos2d::EventListener*	_listener;


	bool	_inGameMode;
};

#endif 
