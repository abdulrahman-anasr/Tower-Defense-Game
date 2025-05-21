#include "Player.h"

Player::Player()
{
  mMoney = 200;
  mHealth = 100;
}

int Player::getMoney()
{
  return mMoney;
}

void Player::incrementMoney(int amount)
{
  mMoney += amount;
}

void Player::decrementMoney(int amount)
{
  mMoney -= amount;
}

void Player::decrementHealth(int points)
{
  mHealth -= points;
}