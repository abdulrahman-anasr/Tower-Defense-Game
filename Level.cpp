#include "Level.h"

#include "Tower.h"


Level::Level(Player* player)
{
  mCurrentPlayer = player;

  mStartingPoint.x = 1460;
  mStartingPoint.y = 950;

  mMapWidth = SCREEN_WIDTH;
  mMapHeight = SCREEN_HEIGHT;

  
}

Level::Level(std::string path, Player* player)
{
  mCurrentPlayer = player;

  mCurrentTimeElapsed = 0.0f;

  mMapWidth = SCREEN_WIDTH, mMapHeight = SCREEN_HEIGHT;

  mMapTexture = nullptr;

  std::string currentLine;

  std::vector<vec3> points;

  std::ifstream file(path);

  while (std::getline(file, currentLine))
  {
    std::string word;
    std::stringstream ss(currentLine);
    std::vector<std::string> stringVector;
    while (getline(ss, word, ' '))
    {
      stringVector.push_back(word);
    }

    if (stringVector[0][0] == 'e')
    {
      EnemyFactory* enemyFactory = new EnemyFactory(stringVector[1]);
      mEnemyFactories.push_back(enemyFactory);
    }
    else if (stringVector[0][0] == 't')
    {
      mLowerIntervalTime = std::stof(stringVector[1]);
      mUpperIntervalTime = std::stof(stringVector[2]);
    }
    else if (stringVector[0][0] == 'm')
    {
      std::string path = stringVector[1];
      
      int width = std::stoi(stringVector[2]);
      int height = std::stoi(stringVector[3]);
      mMapTexture = new Texture(path, width, height, width, height, 1 , 1);
    }
    else if (stringVector[0][0] == 'c')
    {
      TowerFactory* towerFactory = new TowerFactory(stringVector[1]);
      std::string cost = std::to_string(towerFactory->getTowerCost());
      std::stringstream stream(stringVector[1]);
      std::string name;
      getline(stream, name, '.');
      stream.str("");
      stream << name;
      while (getline(stream, name, '/'));
      Button button = { name , cost , {100 , 150} , {0.0f , 0.0f , 0.0f} , towerFactory };
      mButtons.push_back(button);
    }
    else if (stringVector[0][0] == 'b')
    {
      BulletFactory* bulletFactory = new BulletFactory(stringVector[1]);
      mBulletFactories.push_back(bulletFactory);
    }
    else if (stringVector[0][0] == 'p')
    {
      float xPoint = std::stof(stringVector[1]);
      float yPoint = std::stof(stringVector[2]);
      vec3 vector = { xPoint , yPoint };
      points.push_back(vector);
    }
  }

  mStartingPoint = points[0];
  constructPath(mMovementDirector, points);
  constructPathRectangles(mPathRectangles, points);

  mTimeInterval = mLowerIntervalTime + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mUpperIntervalTime - mLowerIntervalTime)));

  for (int i = 0; i < mButtons.size(); i++)
  {
    mButtons[i].factory->setBulletType(mBulletFactories[0]);
  }
}

void Level::moveEnemies(float deltaTime)
{
  //std::cout << "entered loop" << std::endl;
  for (unsigned int i = 0; i < mEnemies.size(); i++)
  {
    int index = mEnemies[i]->getCurrentMovementIndex();
    if (index >= mMovementDirector.size())
    {
      
      /*mCurrentPlayer->decrementHealth(mEnemies[i]->getDamage());
      delete mEnemies[i];
      mEnemies[i] = nullptr;
      mEnemies.erase(mEnemies.begin() + i);*/
      int index = 0;
      mEnemies[i]->setCurrentMovementIndex(index);
      mEnemies[i]->setPosX(mStartingPoint.x);
      mEnemies[i]->setPosY(mStartingPoint.y);
      mEnemies[i]->move(mMovementDirector[index], deltaTime);

      
    }
    else
    {
      mEnemies[i]->move(mMovementDirector[index] , deltaTime);
    }
  }
}

void Level::moveBullets(float deltaTime)
{

  for (int i = 0; i < mBullets.size(); i++)
  {
    mBullets[i]->moveBullet(deltaTime);
  }
}
void Level::drawLevel()
{

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  if (!debugMode)
  {
    std::vector<float> texCoords = mMapTexture->getTextureVertices(0);
    float bottomX = texCoords[0];
    float bottomY = texCoords[1];
    float topX = texCoords[2];
    float topY = texCoords[3];
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, mMapTexture->getTextureId());
    glBegin(GL_QUADS);
    glTexCoord2f(bottomX, bottomY); glVertex3f(0, 0, 0);
    glTexCoord2f(topX, bottomY); glVertex3f(mMapWidth, 0, 0);
    glTexCoord2f(topX, topY); glVertex3f(mMapWidth, mMapHeight, 0);
    glTexCoord2f(bottomX, topY); glVertex3f(0, mMapHeight, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
  }
  else
  {
    for (int i = 0; i < mPathRectangles.size(); i++)
    {
      int x = mPathRectangles[i].x, y = mPathRectangles[i].y, w = mPathRectangles[i].w, h = mPathRectangles[i].h;
      glPushMatrix();
      glColor3f(0.0f, 1.0f, 0.0f);
      glTranslated(x , y , 0);
      glBegin(GL_QUADS);
      glVertex3f(-(w / 2), -(h / 2), 0);
      glVertex3f((w / 2), -(h / 2), 0);
      glVertex3f((w / 2), (h / 2), 0);
      glVertex3f(-(w / 2), (h / 2), 0);
      glEnd();
      glPopMatrix();
    }
  }

  glColor3f(1.0f, 1.0f, 1.0f);

  for (unsigned int i = 0; i < mEnemies.size(); i++)
  {
    mEnemies[i]->draw();
  }

  for (unsigned int i = 0; i < mTowers.size(); i++)
  {
    mTowers[i]->draw();
  }

  for (unsigned int i = 0; i < mBullets.size(); i++)
  {
    mBullets[i]->draw();
  }

  //displayMoneyInformation();
}

int Level::getEnemiesCount()
{
  return mEnemies.size();
}

std::vector<Enemy*>& Level::getEnemies()
{
  return mEnemies;
}

void Level::towerCheckNearbyEnemies()
{
  for (int i = 0; i < mTowers.size(); i++)
  {
    mTowers[i]->findNearEnemy(this);
  }

  for (int i = 0; i < mTowers.size(); i++)
  {
    mTowers[i]->trackEnemy();
  }
}



//TEMP FUNCTIONS IMPLEMENTATIONS
void Level::tempAddTower(Tower* tower)
{
  mTowers.push_back(tower);
}

void Level::tempAddEnemy(Enemy* enemy)
{
  enemy->setPosX(mStartingPoint.x);
  enemy->setPosY(mStartingPoint.y);
  mEnemies.push_back(enemy);
}

void Level::tempSetMovementDirector(std::vector<DirectionMovement> director)
{
  mMovementDirector = director;
}

void Level::addBullet(Bullet* bullet)
{
  mBullets.push_back(bullet);
}

void Level::towerFireBullet()
{
  for (int i = 0; i < mTowers.size(); i++)
  {
    mTowers[i]->fireBullet(this);
  }
}

std::vector<Bullet*>& Level::tempGetBullets()
{
  return mBullets;
}



void Level::discardOutOfRangeBullets()
{
  for (int i = 0; i < mBullets.size(); i++)
  {
    if (mBullets[i]->getPosX() < 0 || mBullets[i]->getPosX() > SCREEN_WIDTH || mBullets[i]->getPosY() > SCREEN_HEIGHT || mBullets[i]->getPosY() < 0)
    {
      delete mBullets[i];
      mBullets.erase(mBullets.begin() + i);
      i--;
    }
  }
}


void Level::checkBulletCollisions()
{
  for (int i = 0; i < mBullets.size(); i++)
  {
    bool collision = mBullets[i]->checkCollision(this);
    if (collision)
    {
      delete mBullets[i];
      mBullets.erase(mBullets.begin() + i);
      i--;
    }
  }
}

void Level::elapseTime(float deltaTime)
{
  for (int i = 0; i < mTowers.size(); i++)
  {
    mTowers[i]->elapseTime(deltaTime);
  }

  mCurrentTimeElapsed += deltaTime;

  float time = mTimeInterval * 10;

  if (mCurrentTimeElapsed >= time)
  {
    spawnEnemy();
    mCurrentTimeElapsed = 0.0f;
    mTimeInterval = mLowerIntervalTime + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mUpperIntervalTime - mLowerIntervalTime)));
  }
}

void Level::discardDeadEnemies()
{
  for (int i = 0; i < mEnemies.size(); i++)
  {
    if (mEnemies[i]->getHealth() <= 0)
    {
      delete mEnemies[i];
      mEnemies.erase(mEnemies.begin() + i);
      i--;
    }
  }
}


void Level::progressLevel(float deltaTime)
{


  discardOutOfRangeBullets();

  if(mMove)
    moveEnemies(deltaTime);

  moveBullets(deltaTime);

  towerCheckNearbyEnemies();

  elapseTime(deltaTime);

  towerFireBullet();

  checkBulletCollisions();

  discardDeadEnemies();
}

void Level::displayMoneyInformation()
{

  std::vector<float> texCoords = mDollarSignTexture->getTextureVertices(0);
  float bottomX = texCoords[0];
  float bottomY = texCoords[1];
  float topX = texCoords[2];
  float topY = texCoords[3];


  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, mDollarSignTexture->getTextureId());
  glTranslated(32, SCREEN_HEIGHT - 32, 0);
  glBegin(GL_QUADS);
  glTexCoord2f(bottomX, bottomY); glVertex3f(-32 , -32 , 0);
  glTexCoord2f(topX, bottomY); glVertex3f(32 , -32, 0);
  glTexCoord2f(topX, topY); glVertex3f(32 , 32 , 0);
  glTexCoord2f(bottomX, topY); glVertex3f(-32 , 32, 0);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);
  glPopMatrix();


  std::stringstream stream;
  stream << mCurrentPlayer->getMoney();
  std::string string = stream.str();

  unsigned char str[1024]{};
  std::copy(string.cbegin(), string.cend(), str);

  glPushMatrix();
  glColor3f(1.0f, 1.0f, 1.0f);
  glRasterPos2i(64, SCREEN_HEIGHT - 40);
  int len = string.length();
  for (int i = 0; i < len; ++i) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
  }
  glPopMatrix();

}

Player* Level::getPlayer()
{
  return mCurrentPlayer;
}

bool Level::checkTowerCollision(Rect towerRect, Rect originalDisplay, Rect projectedDisplay)
{
  for (int i = 0; i < mTowers.size(); i++)
  {
    Rect currentTower = mTowers[i]->getTowerRectangle();


    if (checkRectangleCollision(towerRect, currentTower))
    {
      return true;
    }
  }

  for (int i = 0; i < mPathRectangles.size(); i++)
  {
    Rect currentRectangle = mPathRectangles[i];

    if (checkRectangleCollision(towerRect, currentRectangle))
    {
      return true;
    }
  }

  return false;
}

void Level::tempAddPathRectangles(std::vector<Rect> pathRectangles)
{
  mPathRectangles = pathRectangles;
}

void Level::spawnEnemy()
{

  int size = mEnemyFactories.size();

  int randomNum = (rand() % static_cast<int>(size));

  Enemy* enemy = mEnemyFactories[randomNum]->buildEnemy(mStartingPoint.x, mStartingPoint.y);

  mEnemies.push_back(enemy);
}

std::vector<Button>& Level::getButtons()
{
  return mButtons;
}

std::vector<Tower*>& Level::getTowers()
{
  return mTowers;
}