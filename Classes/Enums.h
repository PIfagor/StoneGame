#ifndef _ENUMS_H_
#define _ENUMS_H_

enum Dir { TOP, BOTTOM, LEFT, RIGHT };
enum SpriteType { NONE, BORDER, SAND, STONE, CRYSTAL, GLASS, USER, GHOSTWALL };


enum ConfigVals {
	MAP_WIDTH = 20,
	MAP_HEIGHT = 16,
	TILE_WIDTH = 45,
	TILE_HEIGHT = 45,
	VIBRATION_POWER = 32000
};

enum Tags { TILE, MENU_TAG };

#endif