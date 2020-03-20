#pragma once
#include <iostream>
#include "GameManager.h"
#include <freeglut.h>
#include <math.h>
#include "clock.h"
#include "Audio.h"

class Input
{
public:
	Input() = default;
	~Input() = default;

	void ProcessInput(GameManager* Game, Audio& audio);

	void MouseClick(int button, int state, int x, int y);

	void MousePassiveMove(int x, int y, GameManager* Game);

	void MouseMove(int x, int y)
	{
		std::cout << "Clicked x: " << x << " | y: " << y << std::endl;
	}
	void KeyboardDown(unsigned char key, int x, int y)
	{
		if (KeyState[key] != INPUT_DOWN || KeyState[key] != INPUT_DOWN_FIRST)
		{
			KeyState[key] = INPUT_DOWN_FIRST;
		}
		/*else
		{
			KeyState[key] = INPUT_DOWN;
		}*/
	}

	void KeyboardUp(unsigned char key, int x, int y)
	{
		KeyState[key] = INPUT_UP;
	}

	void SpecialKeyboardDown(int key, int x, int y)
	{
		SpecialKeyState[key] = INPUT_DOWN_FIRST;
	}

	void SpecialKeyboardUP(int key, int x, int y)
	{
		SpecialKeyState[key] = INPUT_UP;
	}

private:

	// Define Input
	enum InputState
	{
		INPUT_UP,
		INPUT_DOWN,
		INPUT_UP_FIRST,
		INPUT_DOWN_FIRST,
	};

	// Input Container
	InputState KeyState[255];

	InputState SpecialKeyState[255];

	InputState MouseState[3];

	float currentMX = 0;
	float currentMY = 0;

};


