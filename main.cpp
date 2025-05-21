#include "Enemy.h"
#include "Tower.h"
#include "Level.h"
#include "Player.h"
#include "Game.h"
#include "TowerFactory.h"
#include "EnemyFactory.h"
#include "GameExtras.h"
#include <irrKlang.h>


#include <glut.h>
using namespace irrklang;

#define GLUT_KEY_ESCAPE 27

GLuint texID;

float rep = 1.0f;

bool firstLoop = true;

bool debugMode;

Texture* mDollarSignTexture;

int SCREEN_HEIGHT = 1600, SCREEN_WIDTH = 2400;



Game* game = new Game();

bool music = false;

bool pause = false;

ISoundEngine* engine;

Player* player = new Player();

float deltaTime = 0.0f;
float lastFrame = 0.0f;


GLuint tex;

void Display(void) {

	if (!music)
	{
		engine->play2D("assets/sound/music/lastnite.mp3", true);
		music = true;

	}
	glClear(GL_COLOR_BUFFER_BIT);

	game->draw();



	glFlush();
}

void time(int val)
{
	float currentTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;

	deltaTime /= MILLISECOND_FACTOR;

	if (firstLoop)
	{
		deltaTime = 0;
		firstLoop = false;
	}
	if (!pause)
	{


		game->getCurrentLevel()->progressLevel(deltaTime);


		glutPostRedisplay();						// redraw 		
	}

	glutTimerFunc(10, time, 0);
}

void Keyboard(unsigned char key, int x, int y) {
	if (key == GLUT_KEY_ESCAPE)
		exit(EXIT_SUCCESS);

	if (key == 'x')
	{
		game->setHUD(!game->getHUD());
	}

	if (key == 'p')
	{
		pause = !pause;
	}

	if (key == 'u')
	{
		pause = false;
	}

	glutPostRedisplay();
}

void mouseFunction(int button, int state, int x, int y)
{
	game->mouseClick(button, state, x, y);
}

void mousePassiveMotion(int x, int y)
{
	game->mouseHovering(x, y);
}

void windowReshapeFunc(int newWidth, int newHeight) {
	GLdouble aspect = (GLdouble)newWidth / (GLdouble)newHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);

	glViewport(0, 0, newWidth, newHeight);

	SCREEN_WIDTH = newWidth;
	SCREEN_HEIGHT = newHeight;

	glutPostRedisplay();
}


void main(int argc, char** argv) {

	debugMode = false;

	engine = createIrrKlangDevice();

	glutInit(&argc, argv);

	GetDesktopResolution(SCREEN_WIDTH, SCREEN_HEIGHT);

	//SCREEN_HEIGHT -= 100;

	game = new Game();


	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(-1000 , 1000);

	stbi_set_flip_vertically_on_load(true);

	glutCreateWindow("Vampire's Den TD");
	glutFullScreen();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(mouseFunction);
	glutPassiveMotionFunc(mousePassiveMotion);
	//glutReshapeFunc(windowReshapeFunc);

	glutTimerFunc(0, time, 0);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	

	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

	glLineWidth(5);



	glEnable(GL_TEXTURE_2D);

	mDollarSignTexture = new Texture("assets/misc/dollar.png", 1600, 1600, 1600, 1600, 1, 1);

	Level* level = new Level("data/levels/level1.lvl" , player);
	game->setCurrentLevel(level);
	game->setCurrentPlayer(player);
	glutMainLoop();
}