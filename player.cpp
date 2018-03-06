#include "player.hpp"
#include <vector>
#include <stdlib.h>
#include <climits>
#include <algorithm>

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */

    this->side = side;

    this->board = new Board();

}

/*
 * Destructor for the player.
 */
Player::~Player() {

    delete board;

}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
        /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
    // Update board with opponent's move:
    Side opponentsSide = (side == BLACK) ? WHITE : BLACK;
    board->doMove(opponentsMove, opponentsSide); 
    std::vector<Move*> moves;
    std::vector<int> scores;
    // Checks to see if there are legal moves
    if (board->hasMoves(side)) 
    {
        // Go through all possible legal moves
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                // Check for legality
                Move *currentMove = new Move(i, j);
                if (board->checkMove(currentMove, side))
                {
                    // Calculate this move's score (based on location)
                    int score = board->scores[i][j];
                    // Store them (in a vector?)
                    moves.push_back(currentMove);
                    scores.push_back(score);
                    // pushing back by same amount, so indices should be same?
                }
            }
        }
        int max_index = 0;
        if (testingMinimax)
        {
            // Minimax all possible moves, pick move with highest score
            int max_score = INT_MIN;
            Board *m_board = board->copy();
            for(unsigned int i = 0; i < moves.size(); i++)
            {
                int curr_score = minimax(moves[i], m_board, 2, true);
                if (curr_score > max_score)
                {
                    max_score = curr_score;
                    max_index = i;
                }
            }
        }
        else
        { 
            // Go through the possible scores and find maximum
            int max = scores[0];
            for (unsigned int i = 0; i < scores.size(); i++)
            {
                if (scores[i] > max)
                {
                    max = scores[i];
                    max_index = i;
                }
            }
        }
        Move *chosenMove = moves[max_index];
        
        
        // ----CODE FOR RANDOM AI---
        // Pick one at random
        // int randMove = rand() % possible_moves.size();
        // Move *chosenMove = possible_moves[randMove];

        // Update board with chosen move and return
        board->doMove(chosenMove, side);
        return chosenMove;
    }
    // No legal moves - passes
    return nullptr;
}

/*
 * Implements a minimax algorithm for finding the optimal move to play. 
 * (only used in testminimax)
 */
int Player::minimax(Move* move, Board* current, int depth, bool maximizingPlayer) {
    // Switches sides depending on value of maximizingPlayer
    Side curr_side;
    Side other_side;
    if(maximizingPlayer) {

        curr_side = side;
        other_side = (side == BLACK) ? WHITE : BLACK;

    }
    else {
        other_side = side;
        curr_side = (side == BLACK) ? WHITE : BLACK;
    }

    // Base case: at depth limit or at terminal node
    if (depth == 0 || !current->hasMoves(curr_side)) {
        return board->count(curr_side) - board->count(other_side);
    }

    if (maximizingPlayer) {
        
        std::vector<Move*> moves;
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // Check for legality
                Move *currentMove = new Move(i, j);
                if (board->checkMove(currentMove, side)) {
                    // Store legal moves
                    moves.push_back(currentMove);
                }
            }
        }
        // Found all legal possible moves...now minimax them
        // Since we are playing, we want max score we can get
        int min_score = INT_MIN;
        for (unsigned int i = 0; i < moves.size(); i++) {
            Board* new_current = current->copy();
            new_current->doMove(moves[i], curr_side);
            int score = minimax(moves[i], new_current, depth-1, false);
            min_score = max(min_score, score);
            delete new_current;
        }
        return min_score;
    }
    else {
        
        std::vector<Move*> moves;
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // Check for legality
                Move *currentMove = new Move(i, j);
                if (board->checkMove(currentMove, side)) {
                    // Store legal moves
                    moves.push_back(currentMove);
                }
            }
        }
        // Minimaxing all legal moves.
        // Find min score since we want to minimize our loss
        int max_score = INT_MAX;
        for (unsigned int i = 0; i < moves.size(); i++) {
            Board* new_current = current->copy();
            new_current->doMove(moves[i], curr_side);
            int score = minimax(moves[i], new_current, depth-1, true);
            max_score = min(max_score, score);
            delete new_current;
        }
        return max_score;
    }
}