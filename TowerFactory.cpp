#include "TowerFactory.h"

TowerFactory::TowerFactory(std::string path)
{
  std::string currentLine;

  std::ifstream file(path);

  std::getline(file, currentLine);
  mWidth = std::stoi(currentLine);

  std::getline(file, currentLine);
  mHeight = std::stoi(currentLine);

  std::getline(file, currentLine);
  mCost = std::stoi(currentLine);

  std::getline(file, currentLine);
  mRange = std::stoi(currentLine);

  std::getline(file, currentLine);
  mTotalFrames = std::stoi(currentLine);

  std::getline(file, currentLine);
  mFiringCooldownTime = std::stod(currentLine);

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
  int nPerRow = std::stoi(currentLine);

  mTexture = new Texture(texturePath, spriteWidth, spriteHeight, textureWidth, textureHeight, mTotalFrames, nPerRow);

  std::getline(file, currentLine);
  float vecX = std::stod(currentLine);

  std::getline(file, currentLine);
  float vecY = std::stod(currentLine);

  std::getline(file, currentLine);
  float vecZ = std::stod(currentLine);

  mBaseColor = { vecX , vecY , vecZ };

  std::getline(file, currentLine);
  vecX = std::stod(currentLine);

  std::getline(file, currentLine);
  vecY = std::stod(currentLine);

  std::getline(file, currentLine);
  vecZ = std::stod(currentLine);

  mTurretColor = { vecX , vecY , vecZ };

  mTower = new Tower(-500, -500, mWidth, mHeight, mBaseColor, mTurretColor, mTexture, mTotalFrames, mRange, mCost, mFiringCooldownTime , mBulletType);
}


TowerFactory::TowerFactory()
{
  mTower = new Tower(-1 , -1);
}


Tower* TowerFactory::buildTower(int posX, int posY)
{
  Tower* tower = new Tower(posX, posY, mWidth, mHeight, mBaseColor, mTurretColor, mTexture, mTotalFrames, mRange, mCost, mFiringCooldownTime , mBulletType);
  return tower;
}

int TowerFactory::getTowerCost()
{
  return mCost;
}

void TowerFactory::display(int posX, int posY)
{
  mTower->drawHalfBlend(posX, posY);
}

Tower* TowerFactory::getTower()
{
  return mTower;
}

void TowerFactory::displayIllegal(int posX, int posY)
{
  mTower->drawIllegalHalfBlend(posX, posY);
}

void TowerFactory::setBulletType(BulletFactory* bulletFactory)
{
  mBulletType = bulletFactory;
}