#include "BulletFactory.h"
#include "Level.h"


BulletFactory::BulletFactory(std::string path)
{
  std::string currentLine;

  std::ifstream file(path);

  std::getline(file, currentLine);
  mName = currentLine;

  std::getline(file, currentLine);
  mWidth = std::stoi(currentLine);

  std::getline(file, currentLine);
  mHeight = std::stoi(currentLine);

  std::getline(file, currentLine);
  mSpeed = std::stoi(currentLine);

  std::getline(file, currentLine);
  mDamage = std::stoi(currentLine);

  std::getline(file, currentLine);
  std::string texturePath = currentLine;

  std::getline(file, currentLine);
  int spriteWidth = std::stoi(currentLine);

  std::getline(file, currentLine);
  int spriteHeight = std::stoi(currentLine);

  std::getline(file, currentLine);
  int textureWidth = std::stoi(currentLine);

  std::getline(file, currentLine);
  int textureHeight = std::stoi(currentLine);

  std::getline(file, currentLine);
  mTotalFrames = std::stoi(currentLine);

  std::getline(file, currentLine);
  int nPerRow = std::stoi(currentLine);

  mTexture = new Texture(texturePath, spriteWidth, spriteHeight, textureWidth, textureHeight, mTotalFrames, nPerRow);

  std::getline(file, currentLine);
  mSoundPath = currentLine;
}
void BulletFactory::fireBullet(int sourcePosX, int sourcePosY, int targetPosX, int targetPosY , float angle ,  Level* level)
{
  //float slope = calculateSlope(sourcePosX, sourcePosY, targetPosX, targetPosY);




  vec3 vector = { targetPosX - sourcePosX , targetPosY - sourcePosY , 0.0f };
  normalizeVector(vector);


  //std::cout << vector.x << " " << vector.y << " " << vector.z << std::endl;
  Bullet* bullet = new Bullet(angle, sourcePosX, sourcePosY, vector , mName , mWidth , mHeight , mSpeed , mDamage , mTexture , mTotalFrames , mSoundPath);
  level->addBullet(bullet);
  engine->play2D(bullet->getBulletSoundPath().c_str(), false);
}

std::string BulletFactory::getName()
{
  return mName;
}