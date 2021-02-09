#ifndef GAMEITERATIONSCORE_H
#define GAMEITERATIONSCORE_H

struct GameIterationScore
{
    int firstPlayerScore=0;
    int secondPlayerScore=0;
    int thirdPlayerScore=0;

    GameIterationScore(int fps, int sps, int tps);
};

#endif // GAMEITERATIONSCORE_H
