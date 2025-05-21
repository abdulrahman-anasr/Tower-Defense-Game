#include "EnemyFactory.h"

EnemyFactory::EnemyFactory(std::string path)
{
  std::string currentLine;

  std::ifstream file(path);

  std::getline(file, currentLine);
  mWidth = std::stoi(currentLine);

  std::getline(file, currentLine);
  mHeight = std::stoi(currentLine);

  std::getline(file, currentLine);
  mDamage = std::stoi(currentLine);

  std::getline(file, currentLine);
  mSpeed = std::stoi(currentLine);

  std::getline(file, currentLine);
  mHealth = std::stoi(currentLine);

  std::getline(file, currentLine);
  mTotalFrames = std::stoi(currentLine);

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

  mTexture = new Texture(texturePath, spriteWidth, spriteHeight, textureWidth, textureHeight, mTotalFrames , nPerRow);

  std::getline(file, currentLine);
  float vecX = std::stod(currentLine);

  std::getline(file, currentLine);
  float vecY = std::stod(currentLine);

  std::getline(file, currentLine);
  float vecZ = std::stod(currentLine);

  vec3 colorVector = {vecX , vecY , vecZ};

  mColor = colorVector;
}

Enemy* EnemyFactory::buildEnemy(int posX, int posY)
{
  Enemy* enemy = new Enemy(posX, posY, mWidth, mHeight, mDamage, mSpeed, mHealth, mTotalFrames, mTexture, mColor);
  return enemy;
}