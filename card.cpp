#include "card.h"

Card::Card()
{
    this->suit=cardSuit(0);
    this->value=cardValue(0);
}

Card::Card(cardSuit suit, cardValue value)
{
    this->suit=suit;
    this->value=value;
}

Card::Card(Card card, int owner)
{
    this->owner=owner;
    this->suit=card.suit;
    this->value=card.value;
}
