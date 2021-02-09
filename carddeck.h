#ifndef CARDDECK_H
#define CARDDECK_H
#include "card.h"
#include <vector>

struct CardDeck
{
    CardDeck();
    void shuffle();

    std::vector<Card> deck;
};

#endif // CARDDECK_H
