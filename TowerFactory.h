#pragma once

#include "BulletFactory.h"
#include "Tower.h"

class TowerFactory {
  public:
    TowerFactory();

    TowerFactory(std::string path);

    Tower* buildTower(int posX , int posY);

    int getTowerCost();

    void display(int posX , int posY);

    void displayIllegal(int posX, int posY);

    Tower* getTower();

    void setBulletType(BulletFactory* bulletFactory);

  private:

    Tower* mTower;

    Texture* mTexture;

    int mWidth, mHeight;

    int mCost;
    int mRange;

    int mTotalFrames;


    vec3 mBaseColor;
    vec3 mTurretColor;

    float mFiringCooldownTime;

    BulletFactory* mBulletType;
};