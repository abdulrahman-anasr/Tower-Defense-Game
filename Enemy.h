#pragma once
#include "GameExtras.h"
#include "Texture.h"

class Enemy {
  public:
    Enemy(int posX , int posY);

    Enemy(int posX, int posY, int width, int height, int damage, int speed, int health, int totalFrames, Texture* texture, vec3 color);

    Enemy();

    void draw();

    int getPosX();

    int getPosY();

    void setPosX(int x);

    void setPosY(int y);

    void constructEnemy(std::string& path);

    /*void move(std::vector<DirectionMovement>& director , float deltaTime);*/

    void move(DirectionMovement movementDirector , float deltaTime);

    EnemyState getState();

    int getTopLeftX();
    int getTopLeftY();

    int getBottomRightX();
    int getBottomRightY();

    int getCurrentMovementIndex();

    void setCurrentMovementIndex(int index);

    void decreaseHealth(int damage);

    int getHealth();

    int getDamage();

    void changeColor(float r, float g, float b);


  private:
    int mPosX, mPosY;

    int mWidth, mHeight;

    int mDamage;

    int mTopLeftPointX, mTopLeftPointY;

    int mBottomRightX, mBottomRightY;

    int mSpeed;

    int mCurrentMovementIndex;

    vec3 mCurrentDistanceCovered;

    vec3 mMovementVector;

    int mHealth;

    int mFrame;

    int mTotalFrames;

    vec3 mColor;

    Texture* mTexture;

    //std::vector<bool> directorBool;

};