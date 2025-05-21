#pragma once
#include "GameExtras.h"
#include "Enemy.h"
#include "BulletFactory.h"
#include "glut.h"

class Level;

class Tower {
  public:
    Tower(int posX , int posY);

    Tower(int posX, int posY, int width, int height, vec3 baseColor, vec3 turretColor, Texture* texture, int totalFrames, int range, int cost, float firingCooldownTime , BulletFactory* bulletType);

    void draw();

    void trackEnemy();

    void drawHalfBlend(int posX, int posY);

    void drawIllegalHalfBlend(int posX, int posY);

    float getAngle();

    void findNearEnemy(Level* level);

    void elapseTime(float deltaTime);

    void fireBullet(Level* level);

    void printDetails();

    int getCost();

    int getHeight();

    int getWidth();

    int getPosX();

    int getPosY();

    void setPosX(int x);

    void setPosY(int y);

    Rect getTowerRectangle();

    void displayInfo();

  private:

    Texture* mTexture;

    int mFrame;

    int mTotalFrames;

    int mPosX, mPosY;

    int mWidth, mHeight;

    int mCost;

    int mRange;

    float mTurretAngle;

    vec3 mBaseColor;
    vec3 mTurretColor;

    float mFiringCooldownTime;
    float mCurrentFiringCooldownTime;

    BulletFactory* mBulletType;

    Enemy* mTarget;

};