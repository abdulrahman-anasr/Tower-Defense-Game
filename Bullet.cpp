#include "Bullet.h"
#include "Level.h"

Bullet::Bullet(float angle, int posX, int posY, vec3 movementVector, std::string name, int width, int height, int speed, int damage, Texture* texture, int totalFrames, std::string soundPath)
{
  mAngle = angle;
  mPosX = posX;
  mPosY = posY;

  mMovementVector = movementVector;

  mName = name;

  mWidth = width;
  mHeight = height;

  mSpeed = speed;

  mDamage = damage;

  mTexture = texture;

  mTotalFrames = totalFrames;

  mSoundPath = soundPath;

  mFrame = 0;

  if (debugMode)
    mTexture = nullptr;
}

Bullet::Bullet(float angle, int posX, int posY , vec3 movementVector)
{

  //std::cout << "Created Bullet at X: " << posX << "And Y: " << posY << "And slope: " << slope << std::endl;

  mSpeed = 15;

  mMovementVector = movementVector;

  mAngle = angle;

  mWidth = 30;
  mHeight = 12;

  mTexture = nullptr;

  mTexture = new Texture("assets/bullets/normalbullet.png", 150, 12, 30, 12, 5 , 5);

  mTotalFrames = 5;

  mFrame = 0;
  mPosX = posX;
  mPosY = posY;

  mSoundPath = "assets/sound/effects/laser.mp3";


  mDamage = 10;
}

void Bullet::draw()
{

  if (mTexture == nullptr)
  {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslated(mPosX, mPosY, 0);
    glRotated(mAngle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex3f(-(mWidth / 2), -(mHeight / 2), 0);
    glVertex3f(-(mWidth / 2), (mHeight / 2), 0);
    glVertex3f((mWidth / 2), 0, 0);
    glEnd();
    glPopMatrix();
  }
  else
  {
    std::vector<float> texCoords = mTexture->getTextureVertices((mFrame / 12));
    float bottomX = texCoords[0];
    float bottomY = texCoords[1];
    float topX = texCoords[2];
    float topY = texCoords[3];

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, mTexture->getTextureId());
    glTranslated(mPosX, mPosY, 0);
    glRotated(mAngle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(bottomX, bottomY); glVertex3f(-(mWidth / 2), -(mHeight / 2), 0);
    glTexCoord2f(topX, bottomY); glVertex3f((mWidth / 2), -(mHeight / 2), 0);
    glTexCoord2f(topX, topY); glVertex3f((mWidth / 2), (mHeight / 2), 0);
    glTexCoord2f(bottomX, topY); glVertex3f(-(mWidth / 2), (mHeight / 2), 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    mFrame++;
    if (mFrame / 12 >= mTotalFrames)
    {
      mFrame--;
    }
  }
}

void Bullet::moveBullet(float deltaTime)
{

  float distance = mSpeed * deltaTime;
  mPosX = mPosX + (mMovementVector.x * mSpeed);

  mPosY = mPosY + (mMovementVector.y * mSpeed);



}


bool Bullet::checkCollision(Level* level)
{
  std::vector<Enemy*> enemies = level->getEnemies();
  for (int i = 0; i < enemies.size(); i++)
  {
    int enemyTopLeftX = enemies[i]->getTopLeftX();
    int enemyTopLeftY = enemies[i]->getTopLeftY();
    int enemyBottomRightX = enemies[i]->getBottomRightX();
    int enemyBottomRightY = enemies[i]->getBottomRightY();

    bool collision = checkPointRectangleCollision(mPosX, mPosY, enemyTopLeftX, enemyTopLeftY, enemyBottomRightX, enemyBottomRightY);
    if (collision)
    {
      enemies[i]->decreaseHealth(mDamage);
      level->getPlayer()->incrementMoney(mDamage);
      return true;
    }
  }

  return false;
}

int Bullet::getPosX()
{
  return mPosX;
}

int Bullet::getPosY()
{
  return mPosY;
}

std::string Bullet::getBulletSoundPath()
{
  return mSoundPath;
}