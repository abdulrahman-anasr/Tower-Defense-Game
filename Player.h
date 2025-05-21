#pragma once


class Player {
  public:
    Player();

    int getMoney();

    void incrementMoney(int amount);

    void decrementMoney(int amount);

    void decrementHealth(int points);


  private:
    int mMoney;
    int mHealth;

};