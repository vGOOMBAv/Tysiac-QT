#pragma once
#ifndef CARD_H
#define CARD_H

#include <QString>

/*
 *Pik       Pikes
 *Kier      Hearts
 *Trefl     Clovers
 *Karo      Tiles
*/

enum class cardSuit{
    Pikes, Hearts, Clovers, Tiles
};
const QString cardSuitStr[]={
    "Pikes", "Hearts", "Clovers", "Tiles"
};
static QString cardSuitToString(cardSuit suit){
    return cardSuitStr[(int)suit];
}


enum class cardValue{
    Nine, Jack, Queen, King, Ten, Ace
};
static const QString cardValueStr[] = {
    "9", "J", "Q", "K", "10", "A"
};
static QString cardValueToString(cardValue value){
    return cardValueStr[(int)value];
};
static cardValue qstringToCardValue(QString str){
    for(size_t i=0;i<6;i++){
        if(cardSuitStr[i]==str){
            return cardValue(i);
        }
    }
}
static int cardValueToPoints(cardValue value){
    switch (value) {
        case cardValue::Nine:
            return 0;
        case cardValue::Jack:
            return 2;
        case cardValue::Queen:
            return 3;
        case cardValue::King:
            return 4;
        case cardValue::Ten:
            return 10;
        case cardValue::Ace:
            return 11;
    }
}

struct Card{
    Card();
    Card(cardSuit suit, cardValue value);
    Card(Card card, int owner);

    cardSuit  suit;
    cardValue value;
    int owner=-1;
};

#endif // CARD_H
