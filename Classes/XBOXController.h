#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_

// to make win32api compile faster and reduce the size of win headers
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <XInput.h>
#include <cmath>

#include "cocos2d.h"
USING_NS_CC;

// Now, the XInput Library
#pragma comment(lib, "XInput.lib")

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30


class CXBOXController
{
public:
	enum Buttons {
		BUTTON_B,
		BUTTON_Y,
		BUTTON_START,
		BUTTON_BACK,
		BUTTON_LB,
		BUTTON_RB,

		TRIGGER_LEFT,
		TRIGGER_RIGHT,

		// NEVER TOUCH THIS ONE
		BUTTON_NUM
	};

	CXBOXController(int);

	XINPUT_STATE GetState();

	bool IsConnected();

	void Vibrate(int leftVal = 0, int rightVal = 0);

	void test();

	const Vec2 & leftThumbTest();

	void rightThumbTest();

	//////////////////////////////////////////////////////////
	bool getBtnState(Buttons);
	void setBtnState(Buttons, bool);
	bool isBtnPressed(Buttons);
	bool hasBtnBeenPressed(Buttons);


private:

	XINPUT_STATE	_controllerState;
	int				_controllerNum;

	int				_is_btn_pressed[BUTTON_NUM];
};



#endif