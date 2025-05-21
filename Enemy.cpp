#include "Enemy.h"


Enemy::Enemy(int posX, int posY, int width, int height, int damage, int speed, int health, int totalFrames, Texture* texture, vec3 color)
{
  mPosX = posX;
  mPosY = posY;

  mWidth = width;
  mHeight = height;

  mTopLeftPointX = mPosX - (mWidth / 2);
  mTopLeftPointY = mPosY + (mHeight / 2);

  mBottomRightX = mPosX + (mWidth / 2);
  mBottomRightY = mPosY - (mHeight / 2);

  mMovementVector = { 0.0f , 0.0f , 0.0f };


  mDamage = damage;

  mSpeed = speed;

  mHealth = health;

  mTotalFrames = totalFrames;

  mTexture = texture;

  mColor = color;

  mCurrentDistanceCovered = { 0.0f , 0.0f , 0.0f };

  mCurrentMovementIndex = 0;

  mFrame = 0;

  if (debugMode)
  {
    mTexture = nullptr;
  }
}


Enemy::Enemy(int posX, int posY)
{
  mPosX = posX;
  mPosY = posY;

  mColor.x = 0.3f;
  mColor.y = 0.0f;
  mColor.z = 0.0f;

  mWidth = 64;
  mHeight = 64;

  mTopLeftPointX = mPosX - (mWidth / 2);
  mTopLeftPointY = mPosY + (mHeight / 2);

  mBottomRightX = mPosX + (mWidth / 2);
  mBottomRightY = mPosY - (mHeight / 2);

  mCurrentDistanceCovered = { 0.0f , 0.0f , 0.0f };

  mSpeed = 10;

  mDamage = 10;

  mCurrentMovementIndex = 0;

  mTexture = nullptr;

  mTotalFrames = 4;

  mFrame = 0;

  mHealth = 50;
}

void Enemy::draw()
{

  glPointSize(10);

  if (mTexture == nullptr)
  {
    Direction direction = getDirectionVector(mMovementVector);
    float angle = 0.0f;
    if (direction == LEFT)
    {
      angle = 180.0f;
    }
    else if (direction == DOWN)
    {
      angle = 90.0f;
    }
    else if (direction == RIGHT)
    {
      angle = 270.0f;
    }

    glPushMatrix();
    glColor3f(mColor.x, mColor.y, mColor.z);
    glTranslated(mPosX, mPosY, 0);
    glRotated(angle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex3f(-(mWidth / 2), -(mHeight / 2), 0);
    glVertex3f((mWidth / 2), -(mHeight / 2), 0);
    glVertex3f(0, (mHeight / 2), 0);;
    glEnd();
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
  }

  else
  {
    int modifier = 0;
    Direction direction = getDirectionVector(mMovementVector);
    int numberInRow = mTexture->getNumberPerRow();
    if (direction == UP)
    {
      modifier = numberInRow;
    }
    else if (direction == LEFT)
    {
      modifier = numberInRow * 2;
    }
    else if (direction == RIGHT)
    {
      modifier = numberInRow * 3;
    }

    std::vector<float> texCoords = mTexture->getTextureVertices((mFrame / 16) + modifier);
    float bottomX = texCoords[0];
    float bottomY = texCoords[1];
    float topX = texCoords[2];
    float topY = texCoords[3];

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, mTexture->getTextureId());
    glTranslated(mPosX, mPosY, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(bottomX , bottomY); glVertex3f(-(mWidth / 2), -(mHeight / 2), 0);
    glTexCoord2f(topX, bottomY); glVertex3f((mWidth / 2), -(mHeight / 2), 0);
    glTexCoord2f(topX, topY); glVertex3f((mWidth / 2), (mHeight / 2), 0);
    glTexCoord2f(bottomX, topY); glVertex3f(-(mWidth / 2), (mHeight / 2), 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    mFrame++;

    if (mFrame / 16 >= numberInRow)
    {
      mFrame = 0;
    }

  }


}


int Enemy::getCurrentMovementIndex()
{
  return mCurrentMovementIndex;
}


int Enemy::getPosX()
{
  return mPosX;
}

int Enemy::getPosY()
{
  return mPosY;
}

int Enemy::getTopLeftX()
{
  return mTopLeftPointX;
}

int Enemy::getTopLeftY()
{
  return mTopLeftPointY;
}

int Enemy::getBottomRightX()
{
  return mBottomRightX;
}

int Enemy::getBottomRightY()
{
  return mBottomRightY;
}

void Enemy::changeColor(float r, float g, float b)
{
  mColor = { r , g , b };
}

void Enemy::decreaseHealth(int damage)
{
  mHealth -= damage;
}

int Enemy::getHealth()
{
  return mHealth;
}

void Enemy::move(DirectionMovement movementDirector , float deltaTime)
{
  mMovementVector = movementDirector.vec;
  int distanceCoveredX = movementDirector.vec.x * (mSpeed *  deltaTime);
  int distanceCoveredY = movementDirector.vec.y * (mSpeed * deltaTime);
  mPosX += distanceCoveredX;
  mPosY += distanceCoveredY;

  mTopLeftPointX += distanceCoveredX;
  mBottomRightX += distanceCoveredX;

  mTopLeftPointY += distanceCoveredY;
  mBottomRightY += distanceCoveredY;

  distanceCoveredX = abs(distanceCoveredX);
  distanceCoveredY = abs(distanceCoveredY);
  

  mCurrentDistanceCovered.x += distanceCoveredX;
  mCurrentDistanceCovered.y += distanceCoveredY;


  if (mCurrentDistanceCovered.x >= movementDirector.distanceX && mCurrentDistanceCovered.y >= movementDirector.distanceY)
  {
    mCurrentMovementIndex++;
    mCurrentDistanceCovered.x = 0;
    mCurrentDistanceCovered.y = 0;
  }
  
}

int Enemy::getDamage()
{
  return mDamage;
}

void Enemy::setCurrentMovementIndex(int index)
{
  mCurrentMovementIndex = index;
}

void Enemy::setPosX(int x)
{
  mPosX = x;
  mTopLeftPointX = mPosX - (mWidth / 2);
  mBottomRightX = mPosX + (mWidth / 2);
}

void Enemy::setPosY(int y)
{
  mPosY = y;
  mTopLeftPointY = mPosY + (mHeight / 2);
  mBottomRightY = mPosY - (mHeight / 2);
}