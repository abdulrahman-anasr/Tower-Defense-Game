#pragma once

#include "Player.h"
#include "Level.h"
#include "TowerFactory.h"
#include "GameExtras.h"

class Game {
  public:
    Game();

    void setCurrentLevel(Level* level);

    void setCurrentPlayer(Player* player);

    Player* getCurrentPlayer();

    Level* getCurrentLevel();

    void draw();

    void drawHUD();

    void mouseHovering(int posX , int posY);

    void mouseClick(int button, int state, int x, int y);


    void setHUD(bool value);

    void displayInfo();

    bool getHUD();

  private:
    Player* mCurrentPlayer;
    Level* mCurrentLevel;

    int mCurrentLevelNumber;

    int mHUDWidth;

    int mHUDHeight;

    int mMapWidth;

    int mMapHeight;

    bool mDisplayHUD;

    bool mDisplayInfo;

    int mMousePosX;
    int mMousePosY;

    TowerFactory* mSelectedFactory;

    Tower* mSelectedTower;
};