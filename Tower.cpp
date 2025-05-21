#include "Tower.h"

#include "Level.h"

Tower::Tower(int posX , int posY)
{
  mWidth = 64, mHeight = 82;

  mPosX = posX;

  mPosY = posY;

  mTurretAngle = 20;

  mBaseColor = { 0.4f, 0.7f, 0.1f };
  mTurretColor = { 1.0f, 0.0f, 0.0f };

  mTexture = nullptr;
  mTexture = new Texture("assets/textures/witchkitty_look.png", 64, 256, 64, 64, 4 , 1);

  mTotalFrames = 4;

  mFrame = 0;

  mTurretAngle = 0;

  mRange = 1000;

  mCost = 100;

  mFiringCooldownTime = 5.0f;

  mCurrentFiringCooldownTime = 0.0f;

  //mBulletType = new BulletFactory();
}

Tower::Tower(int posX, int posY, int width, int height, vec3 baseColor, vec3 turretColor, Texture* texture, int totalFrames, int range, int cost, float firingCooldownTime , BulletFactory* bulletType)
{
  mPosX = posX, mPosY = posY;
  mWidth = width, mHeight = height;

  mBaseColor = baseColor;
  mTurretColor = turretColor;

  mTexture = texture;

  mTotalFrames = totalFrames;

  mRange = range;

  mCost = cost;

  mFiringCooldownTime = firingCooldownTime;

  mCurrentFiringCooldownTime = 0.0f;

  mBulletType = bulletType;

  mFrame = 0;

  mTarget = nullptr;

  mTurretAngle = 0.0f;

  if (debugMode)
  {
    mTexture = nullptr;
  }
}

void Tower::draw()
{
  mFrame++;

  if (mTexture == nullptr)
  {
    glPushMatrix();
    glColor3f(mBaseColor.x, mBaseColor.y, mBaseColor.z);
    glTranslated(mPosX, mPosY, 0);
    glBegin(GL_QUADS);
    glVertex3f(-(mWidth / 2), -(mHeight / 2), 0);
    glVertex3f((mWidth / 2), -(mHeight / 2), 0);
    glVertex3f((mWidth / 2), (mHeight / 2), 0);
    glVertex3f(-(mWidth / 2), (mHeight / 2), 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(mTurretColor.x, mTurretColor.y, mTurretColor.z);
    glTranslated(mPosX, mPosY, 0);
    glRotated(mTurretAngle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex3f(-(mWidth / 2), -(mHeight / 2), 0);
    glVertex3f(-(mWidth / 2), (mHeight / 2), 0);
    glVertex3f((mWidth / 2), 0, 0);
    glEnd();
    glPopMatrix();
  }

  else
  {

    Direction lookDirection = getDirectionAngle(mTurretAngle);
    std::vector<float> texCoords = mTexture->getTextureVertices(lookDirection);
    float bottomX = texCoords[0];
    float bottomY = texCoords[1];
    float topX = texCoords[2];
    float topY = texCoords[3];

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, mTexture->getTextureId());
    glTranslated(mPosX, mPosY, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(bottomX, bottomY); glVertex3f(-(mWidth / 2), -(mHeight / 2), 0);
    glTexCoord2f(topX, bottomY); glVertex3f((mWidth / 2), -(mHeight / 2), 0);
    glTexCoord2f(topX, topY); glVertex3f((mWidth / 2), (mHeight / 2), 0);
    glTexCoord2f(bottomX, topY); glVertex3f(-(mWidth / 2), (mHeight / 2), 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
  }

  if (debugMode)
  {
    double pi = 3.14159265358979323846;
    glPushMatrix();
    glColor3f(mTurretColor.x, mTurretColor.y, mTurretColor.z);
    glTranslated(mPosX, mPosY, 0.0f);
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < 8; ii++)
    {
      float theta = 2.0f * 3.1415926f * float(ii) / float(8);

      float x = mRange * cosf(theta);
      float y = mRange * sinf(theta);

      glVertex3f(x, y, 0.0f);

    }
    glEnd();
    glPopMatrix();
  }

  glColor3f(1.0f, 1.0f, 1.0f);

  if (mFrame / 64 > mTotalFrames)
  {
    mFrame = 0;
  }
}

void Tower::trackEnemy()
{
  if (mTarget != nullptr)
  {
    vec3 normalDirection = { 1.0f , 0.0f , 0.0f };
    vec3 direction = { mTarget->getPosX() - mPosX , mTarget->getPosY() - mPosY , 0.0f };

    normalizeVector(normalDirection);
    normalizeVector(direction);

    float dot = dotProduct(normalDirection, direction);
    float det = determinent(normalDirection, direction);
    mTurretAngle = toDegrees(atan2(det, dot));
  }
}

float Tower::getAngle()
{
  return mTurretAngle;
}

void Tower::findNearEnemy(Level* level)
{
  int smallestDistance = INT_MAX;
  int smallestDistanceIndex = -1;
  std::vector<Enemy*> levelEnemies = level->getEnemies();
  bool foundTarget = false;
  for (int i = 0; i < level->getEnemiesCount(); i++)
  {
    int enemyPosX = levelEnemies[i]->getPosX();
    int enemyPosY = levelEnemies[i]->getPosY();

    int distance = calculateDistanceSquared(mPosX, mPosY, enemyPosX, enemyPosY);
    int rangeSquared = mRange * mRange;

    if (distance < rangeSquared)
    {
      if (distance < smallestDistance)
      {
        smallestDistance = distance;
        smallestDistanceIndex = i;
      }
    }
  }

  if (smallestDistanceIndex != -1)
  {
    mTarget = levelEnemies[smallestDistanceIndex];
  }
  else
  {
    mTarget = nullptr;
  }
}

void Tower::elapseTime(float deltaTime)
{
  mCurrentFiringCooldownTime += deltaTime;
}

void Tower::fireBullet(Level* level)
{
  if (mTarget != nullptr && mCurrentFiringCooldownTime >= mFiringCooldownTime)
  {
    mBulletType->fireBullet(mPosX, mPosY, mTarget->getPosX() , mTarget->getPosY() , mTurretAngle , level);
    mCurrentFiringCooldownTime = 0.0f;
  }
}

int Tower::getCost()
{
  return mCost;
}

void Tower::drawHalfBlend(int posX , int posY)
{

  if (mTexture == nullptr)
  {
    glPushMatrix();
    glColor4f(mBaseColor.x, mBaseColor.y, mBaseColor.z, 0.5f);
    glTranslated(posX, posY, 0);
    glBegin(GL_QUADS);
    glVertex3f(-(mWidth / 2), -(mHeight / 2), 0);
    glVertex3f((mWidth / 2), -(mHeight / 2), 0);
    glVertex3f((mWidth / 2), (mHeight / 2), 0);
    glVertex3f(-(mWidth / 2), (mHeight / 2), 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor4f(mTurretColor.x, mTurretColor.y, mTurretColor.z, 0.5f);
    glTranslated(posX, posY, 0);
    glBegin(GL_TRIANGLES);
    glVertex3f(-(mWidth / 2), -(mHeight / 2), 0);
    glVertex3f(-(mWidth / 2), (mHeight / 2), 0);
    glVertex3f((mWidth / 2), 0, 0);
    glEnd();
    glPopMatrix();


  }
  else
   {

      std::vector<float> texCoords = mTexture->getTextureVertices(0);
      float bottomX = texCoords[0];
      float bottomY = texCoords[1];
      float topX = texCoords[2];
      float topY = texCoords[3];

      glPushMatrix();
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glBindTexture(GL_TEXTURE_2D, mTexture->getTextureId());
      glTranslated(posX, posY, 0);
      glBegin(GL_QUADS);
      glTexCoord2f(bottomX, bottomY); glVertex3f(-(mWidth / 2), -(mHeight / 2), 0);
      glTexCoord2f(topX, bottomY); glVertex3f((mWidth / 2), -(mHeight / 2), 0);
      glTexCoord2f(topX, topY); glVertex3f((mWidth / 2), (mHeight / 2), 0);
      glTexCoord2f(bottomX, topY); glVertex3f(-(mWidth / 2), (mHeight / 2), 0);
      glEnd();
      glBindTexture(GL_TEXTURE_2D, 0);
      glPopMatrix();
   }
}

void Tower::drawIllegalHalfBlend(int posX, int posY)
{

  if (mTexture == nullptr)
  {
    glPushMatrix();
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
    glTranslated(posX, posY, 0);
    glBegin(GL_QUADS);
    glVertex3f(-(mWidth / 2), -(mHeight / 2), 0);
    glVertex3f((mWidth / 2), -(mHeight / 2), 0);
    glVertex3f((mWidth / 2), (mHeight / 2), 0);
    glVertex3f(-(mWidth / 2), (mHeight / 2), 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
    glTranslated(posX, posY, 0);
    glBegin(GL_TRIANGLES);
    glVertex3f(-(mWidth / 2), -(mHeight / 2), 0);
    glVertex3f(-(mWidth / 2), (mHeight / 2), 0);
    glVertex3f((mWidth / 2), 0, 0);
    glEnd();
    glPopMatrix();
  }
  else
  {
    std::vector<float> texCoords = mTexture->getTextureVertices(0);
    float bottomX = texCoords[0];
    float bottomY = texCoords[1];
    float topX = texCoords[2];
    float topY = texCoords[3];

    glPushMatrix();
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
    glBindTexture(GL_TEXTURE_2D, mTexture->getTextureId());
    glTranslated(posX, posY, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(bottomX, bottomY); glVertex3f(-(mWidth / 2), -(mHeight / 2), 0);
    glTexCoord2f(topX, bottomY); glVertex3f((mWidth / 2), -(mHeight / 2), 0);
    glTexCoord2f(topX, topY); glVertex3f((mWidth / 2), (mHeight / 2), 0);
    glTexCoord2f(bottomX, topY); glVertex3f(-(mWidth / 2), (mHeight / 2), 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
  }
}

int Tower::getWidth()
{
  return mWidth;
}

int Tower::getHeight()
{
  return mHeight;
}

int Tower::getPosX()
{
  return mPosX;
}

int Tower::getPosY()
{
  return mPosY;
}

Rect Tower::getTowerRectangle()
{
  Rect towerRectangle = { mWidth , mHeight , mPosX , mPosY };

  return towerRectangle;
}

void Tower::setPosX(int x)
{
  mPosX = x;
}

void Tower::setPosY(int y)
{
  mPosY = y;
}

void Tower::printDetails() {
  // Print the labels before the values
  std::cout << "Frames: " << mFrame << " ";
  std::cout << "TotalFrames: " << mTotalFrames << " ";
  std::cout << "PosX: " << mPosX << " ";
  std::cout << "PosY: " << mPosY << " ";
  std::cout << "Width: " << mWidth << " ";
  std::cout << "Height: " << mHeight << " ";
  std::cout << "Cost: " << mCost << " ";
  std::cout << "Range: " << mRange << " ";
  std::cout << "TurretAngle: " << mTurretAngle << " ";
  std::cout << "BaseColor: (" << mBaseColor.x << ", " << mBaseColor.y << ", " << mBaseColor.z << ") ";
  std::cout << "TurretColor: (" << mTurretColor.x << ", " << mTurretColor.y << ", " << mTurretColor.z << ") ";
  std::cout << "FiringCooldownTime: " << mFiringCooldownTime << " ";
  std::cout << "CurrentFiringCooldownTime: " << mCurrentFiringCooldownTime << std::endl;
}

void Tower::displayInfo()
{
  glPushMatrix();
  glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
  glBegin(GL_QUADS);
  glVertex3f(SCREEN_WIDTH - 200, 0.0f, 0.0f);
  glVertex3f(SCREEN_WIDTH, 0.0f, 0.0f);
  glVertex3f(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
  glVertex3f(SCREEN_WIDTH - 200, SCREEN_HEIGHT, 0.0f);
  glEnd();
  glPopMatrix();

  int currentX = SCREEN_WIDTH - 200 + ((200) / 2);
  int currentY = SCREEN_HEIGHT - 50;

  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  drawHalfBlend(currentX, currentY);

  currentY -= 70;


  std::string bulletText = "Bullet Type: Base";
  int textWidth = getTextWidth(bulletText);
  renderText(bulletText, currentX - (textWidth / 2), currentY);

  currentY -= 70;

  std::stringstream stream;

  stream << "Range: " << mRange;
  std::string text = stream.str();
  textWidth = getTextWidth(text);
  renderText(text, currentX - (textWidth / 2), currentY);

  currentY -= 70;

  stream.str("");
  stream << "Bullet: " << mBulletType->getName();
  text = stream.str();
  textWidth = getTextWidth(text);
  renderText(text, currentX - (textWidth / 2), currentY);
}
