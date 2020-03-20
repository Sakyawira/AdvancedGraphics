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
		if (Game->IsEnded())
		{
			// Start working on initializing the game on restart instead of making a new one
			Game->Initialize();
		}

		else if (!Game->IsStarted() && !Game->IsEnded())
		{
			Game->StartGame();
		}
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
}
