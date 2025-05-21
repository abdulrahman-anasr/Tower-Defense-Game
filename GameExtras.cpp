#include "GameExtras.h"

GLuint loadTexture(std::string path)
{
  GLuint textureId;
  glGenTextures(1, &textureId);

  int width, height, nrComponents;
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
  if (data)
  {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureId);
    gluBuild2DMipmaps(GL_TEXTURE_2D, nrComponents, width, height, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureId;
}

float toDegrees(float angleInRadians)
{
  float pi = 3.14159265f;
  return (angleInRadians * 180 ) / pi;
}

float toRadians(float angleInDegrees)
{
  float pi = 3.14159265f;
  return (angleInDegrees * pi) / 180;
}

float getLength(vec3& vec)
{
  return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

void normalizeVector(vec3& vec)
{
  float length = getLength(vec);
  vec.x /= length;
  vec.y /= length;
  vec.z /= length;
}

float dotProduct(vec3 vecA, vec3 vecB)
{
  return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
}

float determinent(vec3 vecA, vec3 vecB)
{
  return vecA.x * vecB.y - vecA.y * vecB.x;
}

int squareNumber(int number)
{
  return number * number;
}

bool isWithinRadiusRange(int sourceX, int sourceY, int sourceRadius, int targetX, int targetY)
{
  int radiusSquared = sourceRadius * sourceRadius;

  int distance = squareNumber(targetX - sourceX) + squareNumber(targetY - sourceY);

  if (distance <= radiusSquared)
  {
    return true;
  }

  return false;
}

float calculateSlope(float firstPointX, float firstPointY, float secondPointX, float secondPointY)
{
  float floatFirstX = static_cast<float>(firstPointX), floatFirstY = static_cast<float>(firstPointY);
  float floatSecondX = static_cast<float>(secondPointX), floatSecondY = static_cast<float>(secondPointY);

  std::cout.precision(4);
  

  return (secondPointY - firstPointY) / (secondPointX - firstPointX);
}


bool checkPointRectangleCollision(int pointX, int pointY, int topLeftX, int topLeftY, int bottomRightX, int bottomRightY)
{
  if(pointY < bottomRightY)
  {
    return false;
  }
  if(pointY > topLeftY)
  {
    return false;
  }
  if(pointX > bottomRightX)
  {
    return false;
  }
  if (pointX < topLeftX)
  {
    return false;
  }

  return true;
}


void getNewProjectedPoints(Rect originalDisplay, Rect projectedDisplay, int& posX, int& posY)
{
  float heightRatio = static_cast<float>(originalDisplay.h) / static_cast<float>(projectedDisplay.h);

  float widthRatio = static_cast<float>(originalDisplay.w) / static_cast<float>(projectedDisplay.w);

  posY -= projectedDisplay.y - originalDisplay.y;
  posY = posY * heightRatio;

  posX -= projectedDisplay.x - originalDisplay.x;
  posX = posX * widthRatio;
}


bool checkRectangleCollision(Rect a, Rect b) {
  //The sides of the rectangles
  int leftA, leftB;
  int rightA, rightB;
  int topA, topB;
  int bottomA, bottomB;

  //Calculate the sides of rect A
  leftA = a.x - (a.w / 2);
  rightA = a.x + (a.w / 2);
  topA = a.y + (a.h / 2);
  bottomA = a.y - (a.h / 2);

  //Calculate the sides of rect B
  leftB = b.x - (b.w / 2);
  rightB = b.x + (b.w / 2);
  topB = b.y + (b.h / 2);
  bottomB = b.y - (b.h / 2);
  if (bottomA >= topB) {
    return false;
  }

  if (topA <= bottomB) {
    return false;
  }

  if (rightA <= leftB) {
    return false;
  }

  if (leftA >= rightB) {
    return false;
  }

  return true;
}

void constructPath(std::vector<DirectionMovement>& director, std::vector<vec3> points)
{
  for (int i = 1; i < points.size(); i++)
  {
    vec3 previousPoint = points[i - 1];
    vec3 currentPoint = points[i];

    int distanceX = abs(currentPoint.x - previousPoint.x);
    int distanceY = abs(currentPoint.y - previousPoint.y);

    vec3 movementVector = findDirectionVectorNormalized(previousPoint , currentPoint);

    director.push_back({ movementVector , distanceX , distanceY });
  }
}

void constructPathRectangles(std::vector<Rect>& rectangles, std::vector<vec3> points)
{
  for (int i = 1; i < points.size(); i++)
  {
    vec3 previousPoint = points[i - 1];
    vec3 currentPoint = points[i];

    vec3 midPoint = findMidPoint(previousPoint, currentPoint);
    vec3 dirVec = findDirectionVector(previousPoint, currentPoint);

    dirVec.x = abs(dirVec.x) == 0 ? 50 : abs(dirVec.x);
    dirVec.y = abs(dirVec.y) == 0 ? 50 : abs(dirVec.y);

    Rect rectangle = { dirVec.x, dirVec.y, midPoint.x , midPoint.y };
    rectangles.push_back(rectangle);
  }
}

vec3 findDirectionVector(vec3 firstVector, vec3 secondVector)
{
  vec3 dirVec = { secondVector.x - firstVector.x , secondVector.y - firstVector.y , secondVector.z - firstVector.z };
  return dirVec;
}

vec3 findDirectionVectorNormalized(vec3 firstVector, vec3 secondVector)
{
  vec3 dirVec = findDirectionVector(firstVector, secondVector);
  normalizeVector(dirVec);
  return dirVec;
}

vec3 findMidPoint(vec3 firstVector, vec3 secondVector)
{
  vec3 dirVec = findDirectionVector(firstVector, secondVector);
  dirVec = { firstVector.x + (dirVec.x / 2) , firstVector.y + (dirVec.y / 2) , firstVector.z + (dirVec.z / 2) };

  return dirVec;
}

Direction getDirectionVector(vec3 vector)
{
  if (vector.y == 0.0f)
  {
    if (vector.x < 0.0f)
    {
      return DOWN;
    }
    else
    {
      return RIGHT;
    }
  }
  else if (vector.x == 0.0f)
  {
    if (vector.y < 0.0f)
    {
      return LEFT;
    }
    else
    {
      return UP;
    }
  }

  else
  {
    float absX = abs(vector.x);
    float absY = abs(vector.y);
    if (absX > absY)
    {
      if (vector.x < 0.0f)
      {
        return LEFT;
      }
      else
      {
        return RIGHT;
      }
    }
    else
    {
      if (vector.y < 0.0f)
      {
        return DOWN;
      }
      else
      {
        return UP;
      }
    }
  }
}

Direction getDirectionAngle(float angle)
{
  if (angle >= -45.0f && angle < 45.0f)
  {
    return RIGHT;
  }
  if (angle >= 45.0f && angle < 135.0f)
  {
    return UP;
  }
  if (angle >= -135.0f && angle <= 135.0f)
  {
    return LEFT;
  }
  else
  {
    return DOWN;
  }

}

void GetDesktopResolution(int& horizontal, int& vertical)
{
  RECT desktop;
  // Get a handle to the desktop window
  const HWND hDesktop = GetDesktopWindow();
  // Get the size of screen to the variable desktop
  GetWindowRect(hDesktop, &desktop);
  // The top left corner will have coordinates (0,0)
  // and the bottom right corner will have coordinates
  // (horizontal, vertical)
  horizontal = desktop.right;
  vertical = desktop.bottom;
}

void renderText(std::string text , int posX , int posY)
{
  unsigned char str[1024]{};
  std::copy(text.cbegin(), text.cend(), str);

  glPushMatrix();
  glColor3f(1.0f, 1.0f, 1.0f);
  glRasterPos2i(posX, posY);
  int len = text.length();
  for (int i = 0; i < len; ++i) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
  }
  glPopMatrix();
}

int getTextWidth(std::string text)
{
  int width = 0;
  for (int i = 0; i < text.length(); i++)
  {
    char currentChar = text[i];
    width += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, currentChar);
  }

  return width;
}

int calculateDistanceSquared(int sourceX, int sourceY, int targetX, int targetY)
{
  return squareNumber(targetX - sourceX) + squareNumber(targetY - sourceY);
}