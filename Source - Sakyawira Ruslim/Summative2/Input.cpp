#include "Input.h"

void Input::ProcessInput(GameManager* Game, Audio& audio)
{
	if (KeyState['w'] == INPUT_DOWN_FIRST)
	{
		Game->tank->Rotate(0.0f);
		KeyState['w'] = INPUT_DOWN;
	}

	if (KeyState['s'] == INPUT_DOWN_FIRST)
	{
		Game->tank->Rotate(180.0f);
		KeyState['s'] = INPUT_DOWN;
	}

	if (KeyState['a'] == INPUT_DOWN_FIRST)
	{
		Game->tank->Rotate(90.f);
		KeyState['a'] = INPUT_DOWN;
	}

	if (KeyState['d'] == INPUT_DOWN_FIRST)
	{
		Game->tank->Rotate(-90.f);
		KeyState['d'] = INPUT_DOWN;
	}

	if (KeyState[32] == INPUT_DOWN_FIRST)
	{
		glm::vec3 _target = Game->tank->GetLocation();
	}

	if (KeyState['w'] == INPUT_DOWN)
	{
		Game->tank->Move(MOVE_FRONT, 0.5f * Game->GetClock()->GetDeltaTick() * 120.0f);
		Game->tank->m_currently_moved = true;
		Game->camera.MovePosZ(15.0f, Game->GetClock()->GetDeltaTick());
	}
	if (KeyState['s'] == INPUT_DOWN)
	{
		Game->tank->Move(MOVE_BACK, 0.5f * Game->GetClock()->GetDeltaTick() * 120.0f);
		Game->tank->m_currently_moved = true;
		Game->camera.MovePosZ(-15.0f, Game->GetClock()->GetDeltaTick());
	}
	if (KeyState['d'] == INPUT_DOWN)
	{
		Game->tank->Move(MOVE_RIGHT, 0.5f * Game->GetClock()->GetDeltaTick() * 120.0f);
		Game->tank->m_currently_moved = true;
		Game->camera.MovePosX(-15.0f, Game->GetClock()->GetDeltaTick());
	}
	if (KeyState['a'] == INPUT_DOWN)
	{
		Game->tank->Move(MOVE_LEFT, 0.5f * Game->GetClock()->GetDeltaTick() * 120.0f);
		Game->tank->m_currently_moved = true;
		Game->camera.MovePosX(15.0f, Game->GetClock()->GetDeltaTick());
	}
	if (KeyState['r'] == INPUT_DOWN_FIRST)
	{
		if (!Game->IsStarted() /*&& !Game->IsEnded()*/)
		{
			Game->StartGame(true);
		}
		else
		{
			// Start working on initializing the game on restart instead of making a new one
			Game->StartGame(false);
			Game->Initialize();
		}
		KeyState['r'] = INPUT_DOWN;
	}

	if (KeyState['e'] == INPUT_DOWN)
	{
		//Game->camera.MovePosY(-0.1f * Game->GetClock()->GetDeltaTick() * 120.0f);
	}

	if (KeyState['q'] == INPUT_DOWN)
	{
	//	Game->camera.MovePosY(0.1f * Game->GetClock()->GetDeltaTick() * 120.0f);
	}

	//if (SpecialKeyState[GLUT_KEY_UP] == INPUT_DOWN || KeyState['w'] == INPUT_DOWN)
	//{
	//	Game->camera.MovePosZ(-0.1f * Game->GetClock()->GetDeltaTick() * 120.0f);
	//}

	//if (SpecialKeyState[GLUT_KEY_DOWN] == INPUT_DOWN || KeyState['s'] == INPUT_DOWN)
	//{
	//	Game->camera.MovePosZ(0.1f * Game->GetClock()->GetDeltaTick() * 120.0f);
	//}

	//if (SpecialKeyState[GLUT_KEY_LEFT] == INPUT_DOWN || KeyState['a'] == INPUT_DOWN)
	//{
	//	Game->camera.MovePosX(-0.1f * Game->GetClock()->GetDeltaTick() * 120.0f);
	//}

	//if (SpecialKeyState[GLUT_KEY_RIGHT] == INPUT_DOWN || KeyState['d'] == INPUT_DOWN)
	//{
	//	Game->camera.MovePosX(0.1f * Game->GetClock()->GetDeltaTick() * 120.0f);
	//}
	if (MouseState[0] == INPUT_DOWN)
	{
		Game->set_click(true);
	}
	else
	{
		Game->set_click(false);
	}
	
}

void Input::MouseClick(int button, int state, int x, int y)
{
	if (button >= 3)
	{
		return;
	}

	MouseState[button] = (state == GLUT_DOWN) ? INPUT_DOWN : INPUT_UP;
	//std::cout << "Clicked Once x: " << x << " | y: " << y << std::endl;
}

void Input::MousePassiveMove(int x, int y, GameManager * Game)
{
	/*For Mouse Picking*/
	const float currentMX = (2.0f * x) / static_cast<float>(800) - 1.0f;
	const float currentMY = 1.0f - (2.0f * y) / static_cast<float>(800);
	Game->set_mouse_pos(glm::vec2(currentMX, currentMY));
	

	/*For Free Moving Camera*/
	//Convert current screen width and height mouse co-ords to 
	x -= 400;
	y -= 400;

	//Invert y axis
	y *= -1;

	//std::cout << "Passive x: " << x << " | y: " << y << std::endl;

	if (Game->IsStarted())
	{
		Game->camera.updateLookDir(x, y);
	}
}

void Input::MouseMove(int x, int y)
{
	//std::cout << "Clicked x: " << x << " | y: " << y << std::endl;
}

void Input::KeyboardDown(unsigned char key, int x, int y)
{
	if (KeyState[key] != INPUT_DOWN || KeyState[key] != INPUT_DOWN_FIRST)
	{
		KeyState[key] = INPUT_DOWN_FIRST;
	}
}

void Input::KeyboardUp(unsigned char key, int x, int y)
{
	KeyState[key] = INPUT_UP;
}

void Input::SpecialKeyboardDown(int key, int x, int y)
{
	SpecialKeyState[key] = INPUT_DOWN_FIRST;
}

void Input::SpecialKeyboardUP(int key, int x, int y)
{
	SpecialKeyState[key] = INPUT_UP;
}
