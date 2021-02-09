#include "player.h"

Player::Player()
{
    this->name="Dummy";
    this->score=0;
}

Player::Player(QString name)
{
    this->name=name;
    this->score=0;
}
