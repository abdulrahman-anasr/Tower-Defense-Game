#pragma once

#include "GameExtras.h"
#include "Texture.h"
#include "glut.h"

class Level;

class Bullet {
  public:
    Bullet( float angle , int posX, int posY , vec3 movementVector);

    Bullet(float angle, int posX, int posY, vec3 movementVector, std::string name, int width, int height, int speed, int damage, Texture* texture, int totalFrames, std::string soundPath);

    void moveBullet(float deltaTime);

    void draw();

    int getPosX();

    int getPosY();

    std::string getBulletSoundPath();


    bool checkCollision(Level* level);

  private:
    std::string mName;

    float mPosX, mPosY;

    int mWidth, mHeight;

    int mSpeed;

    int mDamage;

    vec3 mMovementVector;

    float mAngle;

    Texture* mTexture;

    int mFrame;

    int mTotalFrames;

    std::string mSoundPath;
};