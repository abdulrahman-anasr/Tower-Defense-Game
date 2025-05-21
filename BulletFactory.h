#pragma once
#include "GameExtras.h"
#include "Bullet.h"
#include "irrKlang.h"
using namespace irrklang;

class Level;

class BulletFactory {
  public:
    BulletFactory(std::string path);

    void fireBullet(int sourcePosX , int sourcePosY , int targetPosX , int targetPosY , float angle , Level* level);

    std::string getName();
  private:
    std::string mName;

    int mWidth, mHeight;

    int mSpeed;

    int mDamage;

    Texture* mTexture;

    int mTotalFrames;

    std::string mSoundPath;
};