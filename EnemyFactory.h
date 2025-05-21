#pragma once
#include "GameExtras.h"
#include "Enemy.h"
#include "Texture.h"

class EnemyFactory {
  public:

  EnemyFactory(std::string path);

  Enemy* buildEnemy(int posX, int posY);

  private:
    int mWidth, mHeight;

    int mDamage;

    int mSpeed;

    int mHealth;

    int mTotalFrames;

    Texture* mTexture;

    vec3 mColor;
};