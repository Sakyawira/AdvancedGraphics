#include "Initialization.h"
#include "TextLabel.h"
#include <random>

void Update()
{
	//FMOD_RESULT System::update();
	audio.Update();

	// Process Game
	Game->ProcessGame(audio);
	
	// Process Input
	input.ProcessInput(Game, audio);
	
	// Update game information
	glutPostRedisplay();
}

void KeyboardUP(unsigned char key, int x, int y)
{
	input.KeyboardUp(key, x, y);
}

void KeyboardDOWN(unsigned char key, int x, int y)
{
	input.KeyboardDown(key, x, y);
}

void SpecialKeyUp(int key, int x, int y)
{
	input.SpecialKeyboardUP(key, x, y);
}

void SpecialKeyDown(int key, int x, int y)
{
	input.SpecialKeyboardDown(key, x, y);
}

void MouseClick(int button, int state, int x, int y)
{
	input.MouseClick(button, state, x, y);
}

void MousePassiveMove(int x, int y)
{
	input.MousePassiveMove(x, y, Game);
}

void MouseMove(int x, int y)
{
	input.MouseMove(x, y);
}

int main(int argc, char **argv)
{
	//assert(AudioInit() == true);
	audio.Load();
	audio.Play(SOUND_BGM1); 
	//audio.Play(SOUND_VICTORY);
		
	// Setup and create at glut controlled window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(WINDOW_WIDHT, WINDOW_HEIGHT);
	glutCreateWindow("OpenGL First Window");

	// Sets up all gl function callbacks based on pc hardware

	if (glewInit() != GLEW_OK)
	{
		// If glew setup failed then application will not run graphics correctly
		std::cout << "Glew Initialization Failed. Aborting Application." << std::endl;
		system("pause");
	}

	// Set the clear color when calling glClear()
	glClearColor(0.0, 0.0, 0.0, 1.0); // Red

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glutSetOption(GLUT_MULTISAMPLE, 8);
	glEnable(GL_MULTISAMPLE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	//// Swap culling
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	Game = new GameManager();
	//	Game->Initialize();
	
	//Register callbacks
	glutDisplayFunc(Render);
	glutIdleFunc(Update);

	glutKeyboardFunc(KeyboardDOWN);
	glutKeyboardUpFunc(KeyboardUP);

	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);

	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMove);
	glutPassiveMotionFunc(MousePassiveMove);

	//glutCloseFunc(ShutDown);
	glutMainLoop(); //Ensure this is the last glut line called

	ShutDown();

	return 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	
	if (Game != nullptr)
	{
		Game->Render();
	}

	glutSwapBuffers();
}

void ShutDown()
{
	delete Game;
}
