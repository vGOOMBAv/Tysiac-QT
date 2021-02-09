#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <vector>
#include <card.h>

struct Player
{

    Player();
    Player(QString name);

    QString name;
    size_t scoreToAchieve=0;
    size_t score;
    std::vector<Card> playerCards;
};

#endif // PLAYER_H
