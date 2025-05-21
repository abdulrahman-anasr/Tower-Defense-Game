#pragma once

#include "GameExtras.h"
#include "Bullet.h"
#include "EnemyFactory.h"
#include "TowerFactory.h"
#include "Enemy.h"
#include "Player.h"


class Tower;

class Level {
  public:

    Level(Player* player);

    Level(std::string path , Player* player);

    void startLevel();
    
    void moveEnemies(float deltaTime);

    void moveBullets(float deltaTime);

    void towerCheckNearbyEnemies();

    void addBullet(Bullet* bullet);

    void towerFireBullet();

    void drawLevel();

    int getEnemiesCount();

    void discardOutOfRangeBullets();

    void discardDeadEnemies();

    void checkBulletCollisions();

    std::vector<Enemy*>& getEnemies();

    void progressLevel(float deltaTime);

    void elapseTime(float deltaTime);

    Player* getPlayer();

    void displayMoneyInformation();

    bool checkTowerCollision(Rect towerRect , Rect originalDisplay , Rect projectedDisplay);

    void spawnEnemy();

    std::vector<Button>& getButtons();

    std::vector<Tower*>& getTowers();


    //TEMP FUNCTIONS
    void tempAddTower(Tower* tower);

    void tempAddPathRectangles(std::vector<Rect> pathRectangles);

    void tempAddEnemy(Enemy* enemy);

    void tempSetMovementDirector(std::vector<DirectionMovement> director);

    std::vector<Bullet*>& tempGetBullets();


    bool mMove;

    

  private:

    Texture* mMapTexture;

    int mMapWidth;

    int mMapHeight;

    float  mLowerIntervalTime;

    float mUpperIntervalTime;

    float mCurrentTimeElapsed;

    float mTimeInterval;

    std::vector<EnemyFactory*> mEnemyFactories;

    std::vector<BulletFactory*>  mBulletFactories;

    std::vector<Button> mButtons;

    std::vector<Enemy*> mEnemies;

    std::vector<Rect> mPathRectangles;

    std::vector<Bullet*> mBullets;

    std::vector<Tower*> mTowers;

    std::vector<DirectionMovement> mMovementDirector;

    bool mLevelFinished;

    Player* mCurrentPlayer;

    vec3 mStartingPoint;
};