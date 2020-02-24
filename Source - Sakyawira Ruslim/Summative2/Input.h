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

	void MouseClick(int button, int state, int x, int y)
	{
		
		if (button >= 3)
		{
			return;
		}

		MouseState[button] = (state == GLUT_DOWN) ? INPUT_DOWN : INPUT_UP;
		std::cout << "Clicked Once x: " << x << " | y: " << y << std::endl;
	}

	void MousePassiveMove(int x, int y, GameManager* Game)
	{
		//Convert current screen width and height mouse co-ords to 
			//Move co-ords from (0, 0) at top left, to (0, 0) at middle of screen
			//(remaps screen size mouse coords to opengl pixel coords)
		//x = (int)(Math::remap(x, -800, 800, -800, 800) - 800);
		//y = (int)(Math::remap(y, -800, 800, -800, 800) - 800);

		x -= 400;
		y -= 400;

		//Invert y axis
		y *= -1;

	
		std::cout << "Passive x: " << x << " | y: " << y << std::endl;
		//currentMX = x;
		//currentMY = y;


		Game->camera.updateLookDir(x, y);

		
	}

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


