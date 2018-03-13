#include "player.hpp"
#include <vector>
#include <stdlib.h>
#include <climits>
#include <algorithm>
#include <float.h>

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

     // Stores the current side
     this->side = side;
     // Creates a board (this is what the AI tracks)
     this->board = new Board();

     // ----CODE FOR RANDOM AI----
     // Initialize random seed (this is a test!)
     // srand(time(NULL));
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
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
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
    std::vector<double> scores;
    //std::vector<int> frontier;
    // Checks existence of legal moves
    if (board->hasMoves(side)) {
        // Iterate through all possible moves
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // Check for legality
                Move *currentMove = new Move(i, j);
                if (board->checkMove(currentMove, side)) {
                    // Calculate this move's score based on location
                    Board *newBoard = board->copy();
                    newBoard->doMove(currentMove, side);
                    int score = board->scores[i][j];
                    // Calculate move's frontier score
                    int front_score = 0;
                    for (int a = i - 1; a < i + 2; a++) {
                        for (int b = j - 1; b < j + 2; b++) {
                            if (board->onBoard(a, b) && a != b) {
                                if (board->occupied(a, b))
                                    front_score++;
                            }
                        }
                    }
                    // Calculate weighted average depending on player side
                    double new_score;
                    if (side == WHITE)
                        new_score = front_score * 0.3 + score * 0.7;
                    else
                        new_score = front_score * 0.4 + score * 0.6;
                    // Stores both the move and the weighted score
                    moves.push_back(currentMove);
                    scores.push_back(new_score);
                    delete newBoard;
                }
            }
        }
        int max_index = 0;
        if (testingMinimax) {
            // Minimax possible moves, 
            // Pick move with max score
            double max_score = -DBL_MAX;
            for(unsigned int i = 0; i < moves.size(); i++) {
                Board *m_board = board->copy();
                m_board->doMove(moves[i], side);
                double curr_score = minimax(moves[i], m_board, 2, true);
                if (curr_score > max_score) {
                    max_score = curr_score;
                    max_index = i;
                }
                delete m_board;
            }
        }
        else { 
            // Iterate through the possible scores and 
            // Find max score
            int max = scores[0];
            for (unsigned int i = 0; i < scores.size(); i++) {
                if (scores[i] > max) {
                    max = scores[i];
                    max_index = i;
                }
            }
        }
        Move *chosenMove = moves[max_index];

        // Update board
        board->doMove(chosenMove, side);
        return chosenMove;
    }
    // If no legal moves, then pass
    return nullptr;
}

/*
 * Implements a minimax algorithm for finding the optimal move to play. 
 * (only used in testminimax)
 */
int Player::minimax(Move* move, Board* current, int depth, bool maximizingPlayer) {
    
    // Side depends on value of maximizingPlayer
    Side curr_side;
    Side other_side;
    if(maximizingPlayer) {
        curr_side = side;
        other_side = (curr_side == BLACK) ? WHITE : BLACK;
    }
    else {
        other_side = side;
        curr_side = (other_side == BLACK) ? WHITE : BLACK;
    }
    
    // Base case: at depth limit or at terminal node
    if (depth == 0 || !current->hasMoves(curr_side)) {
        return board->count(curr_side) - board->count(other_side);
    }
    
    if (maximizingPlayer) {
        std::vector<Move*> moves;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // Check if legal
                Move *currentMove = new Move(i, j);
                if (board->checkMove(currentMove, curr_side)) {
                    // Store legal moves
                    moves.push_back(currentMove);
                }
                delete currentMove;
            }
        }
        // Minimax all possible moves
        // Take max score
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
                if (board->checkMove(currentMove, curr_side)) {
                    // Store legal moves
                    moves.push_back(currentMove);
                }
                delete currentMove;
            }
        }
        // Minimax all possible moves
        // Take min score
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