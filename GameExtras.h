#ifndef GAME_EXTRA_NAMESPACE
#define GAME_EXTRA_NAMESPACE
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <array>
#include <sstream>
#include <cstring>
#include <random>
#include <irrKlang.h>
#include "wtypes.h"
#include "stb_image.h"
#include "glut.h"

class Texture;

using namespace irrklang;

class TowerFactory;

const int MILLISECOND_FACTOR = 50;

extern int SCREEN_HEIGHT, SCREEN_WIDTH;

extern bool debugMode;

extern Texture* mDollarSignTexture;

extern ISoundEngine* engine;

struct vec3
{
   float x;
   float y;
   float z;
};

struct Rect
{
  int w;
  int h;
  int x;
  int y;
};

struct Button
{
  std::string name;
  std::string cost;
  Rect rectangle;
  vec3 color;
  TowerFactory* factory;
};

enum Direction
{
  DOWN,
  UP,
  LEFT,
  RIGHT
};

struct DirectionMovement
{
  vec3 vec;
  int distanceX;
  int distanceY;
};

enum EnemyState
{
  MOVING,
  IDLE,
  FINISHED
};

GLuint loadTexture(std::string path);

float getLength(vec3& vec);

void normalizeVector(vec3& vec);

float dotProduct(vec3 vecA, vec3 vecB);

float determinent(vec3 vecA, vec3 vecB);

float toDegrees(float angleInRadians);

float toRadians(float angleInDegrees);

int squareNumber(int number);

float calculateSlope(float firstPointX, float firstPointY, float secondPointX, float secondPointY);

bool isWithinRadiusRange(int sourceX, int sourceY, int sourceRadius, int targetX, int targetY);

int calculateDistanceSquared(int sourceX, int sourceY, int targetX, int targetY);


bool checkPointRectangleCollision(int pointX, int pointY, int topLeftX, int topLeftY, int bottomRightX, int bottomRightY);

void getNewProjectedPoints(Rect originalDisplay , Rect projectedDisplay , int& posX, int& posY);

bool checkRectangleCollision(Rect a, Rect b);

void constructPath(std::vector<DirectionMovement>& director , std::vector<vec3> points);

vec3 findDirectionVector(vec3 firstVector, vec3 secondVector);

vec3 findMidPoint(vec3 firstVector, vec3 secondVector);

vec3 findDirectionVectorNormalized(vec3 firstVector, vec3 secondVector);

void constructPathRectangles(std::vector<Rect>& rectangles, std::vector<vec3> points);


Direction getDirectionVector(vec3 vector);

Direction getDirectionAngle(float angle);

void GetDesktopResolution(int& horizontal, int& vertical);

void renderText(std::string text , int posX , int posY);

int getTextWidth(std::string text);

#endif // !GAME_EXTRA_NAMESPACE