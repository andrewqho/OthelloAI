#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);
    int minimax(Move *move, Board *current, int depth, bool player);
    
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Side side;
    Board *board;
};

#endif