#include "Game.h"


Game::Game()
{
  mCurrentLevelNumber = 1;

  mHUDHeight = 150;

  mHUDWidth = SCREEN_WIDTH;

  mMapWidth = SCREEN_WIDTH;

  mMapHeight = SCREEN_HEIGHT - mHUDHeight;

  mSelectedFactory = nullptr;

  mDisplayHUD = false;

  mSelectedTower = nullptr;

}


void Game::setCurrentLevel(Level* level)
{
  mCurrentLevel = level;
}

void Game::setCurrentPlayer(Player* player)
{
  mCurrentPlayer = player;
}

Player* Game::getCurrentPlayer()
{
  return mCurrentPlayer;
}

Level* Game::getCurrentLevel()
{
  return mCurrentLevel;
}

void Game::draw()
{

  glColor3f(1.0f, 1.0f, 1.0f);

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  mCurrentLevel->drawLevel();



  if(mDisplayHUD)
    drawHUD();

  if (mSelectedTower != nullptr)
  {
    mSelectedTower->displayInfo();
  }

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  if (mSelectedFactory != nullptr)
  {

    Rect originalDisplay = { SCREEN_WIDTH , SCREEN_HEIGHT , 0 , 0 };

    Rect towerRect = mSelectedFactory->getTower()->getTowerRectangle();



    towerRect.x = mMousePosX;
    towerRect.y = mMousePosY;

    //std::cout << "x: " << mMousePosX << " y: " << mMousePosY << std::endl;
    if (mCurrentLevel->checkTowerCollision(towerRect, originalDisplay, originalDisplay))
    {
      mSelectedFactory->displayIllegal(mMousePosX, mMousePosY);
    }
    else
    {
      mSelectedFactory->display(mMousePosX, mMousePosY);
    }
  }

  

}

void Game::drawHUD()
{

  glPushMatrix();
  glColor4f(0.0f, 0.0f, 0.0f , 0.5f);


  glBegin(GL_QUADS);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(mHUDWidth, 0.0f, 0.0f);
  glVertex3f(mHUDWidth, mHUDHeight, 0.0f);
  glVertex3f(0.0f, mHUDHeight, 0.0f);
  glEnd();
  glPopMatrix();



  std::vector<Button>& mButtons = mCurrentLevel->getButtons();
  int totalButtonsWidth = 0;
  for (int i = 0; i < mButtons.size(); i++)
  {
    totalButtonsWidth += mButtons[i].rectangle.w;
  }

  int currentX = (mHUDWidth / 2) - (totalButtonsWidth / 2);
  int currentY = 0;
  for (int i = 0; i < mButtons.size(); i++)
  {
    Button& button = mButtons[i];
    glPushMatrix();
    glColor4f(button.color.x, button.color.y, button.color.z, 0.7f);
    glBegin(GL_QUADS);
    glVertex3f(currentX, currentY, 0.0f);
    glVertex3f(currentX + button.rectangle.w, currentY, 0.0f);
    glVertex3f(currentX + button.rectangle.w, currentY + button.rectangle.h, 0.0f);
    glVertex3f(currentX, currentY + button.rectangle.h, 0.0f);
    glEnd();
    glPopMatrix();

    int midPointX = currentX + (button.rectangle.w / 2);
    int midPointY = currentY + (button.rectangle.h / 1.333);

    button.factory->getTower()->setPosX(midPointX);
    button.factory->getTower()->setPosY(midPointY);

    glColor3f(1.0f, 1.0f, 1.0f);
    button.factory->getTower()->drawHalfBlend(midPointX , midPointY);

    midPointY = currentY + (button.rectangle.h / 1.333);

    int textWidth = getTextWidth(button.cost);
    renderText(button.cost, midPointX - (textWidth / 2), 25);
    

    textWidth = getTextWidth(button.name);
    renderText(button.name, midPointX - (textWidth / 2), 5);

    button.rectangle.x = currentX;
    button.rectangle.y = 0;
    currentX = currentX + button.rectangle.w;
  }

  mCurrentLevel->displayMoneyInformation();
}

void Game::mouseHovering(int posX ,int posY)
{
  posY = SCREEN_HEIGHT - posY;

  mMousePosX = posX;
  mMousePosY = posY;
}

void Game::mouseClick(int button, int state, int x, int y)
{
  y = SCREEN_HEIGHT - y;
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    if (checkPointRectangleCollision(x, y, 0, mHUDHeight, mHUDWidth, 0) && mDisplayHUD)
    {
      std::vector<Button> mButtons = mCurrentLevel->getButtons();
      for (int i = 0; i < mButtons.size(); i++)
      {
        Button button = mButtons[i];
        if (checkPointRectangleCollision(x, y, button.rectangle.x, button.rectangle.h, button.rectangle.x + button.rectangle.w, button.rectangle.y))
        {
          mSelectedFactory = button.factory;
        }
      }
    }
    else
    {
      if (mSelectedFactory != nullptr && mCurrentPlayer->getMoney() >= mSelectedFactory->getTowerCost())
      {

        Rect originalDisplay = { SCREEN_WIDTH , SCREEN_HEIGHT , 0 , 0 };

        Rect towerRect = mSelectedFactory->getTower()->getTowerRectangle();
        towerRect.x = x;
        towerRect.y = y;
        if (!(mCurrentLevel->checkTowerCollision(towerRect , originalDisplay , originalDisplay)))
        {
          Tower* tower = mSelectedFactory->buildTower(x , y);
          mCurrentLevel->tempAddTower(tower);
          mCurrentPlayer->decrementMoney(mSelectedFactory->getTowerCost());
        }
        mSelectedFactory = nullptr;
      }
      else
      {
        std::vector<Tower*>& towers = mCurrentLevel->getTowers();
        for (int i = 0; i < towers.size(); i++)
        {
          Rect towerRect = { towers[i]->getWidth() , towers[i]->getHeight() , towers[i]->getPosX() - (towers[i]->getWidth() / 2) , towers[i]->getPosY() - (towers[i]->getHeight() / 2) };

          if (checkPointRectangleCollision(x, y, towerRect.x, towerRect.y + towerRect.h, towerRect.x + towerRect.w, towerRect.y))
          {
            mSelectedTower = towers[i];
            break;
          }
        }
      }
    }
  }
  else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
  {
    mSelectedFactory = nullptr;
    mSelectedTower = nullptr;
  }

}


void Game::setHUD(bool value)
{
  mDisplayHUD = value;
}

bool Game::getHUD()
{
  return mDisplayHUD;
}
