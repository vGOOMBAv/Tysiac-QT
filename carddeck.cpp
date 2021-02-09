#pragma once
#include "carddeck.h"
#include <algorithm>
#include <random>
#include <QDebug>

CardDeck::CardDeck()
{
    for(size_t i=0;i<4;i++){
        for(size_t j=0;j<6;j++){
            deck.push_back(*(new Card(cardSuit(i), cardValue(j))));
        }
    }
}

void CardDeck::shuffle()
{
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(deck), std::end(deck), rng);
}
