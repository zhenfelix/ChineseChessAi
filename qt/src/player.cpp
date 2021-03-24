#include "player.h"

Player::Player(bool isHuman_, chessboard &cb_)
    : isHuman(isHuman_), cb(cb_)
    {}

void Player::think()
{
    // std::cout << "dummy move in player!" << std::endl;
    // cb.dummy_move();
    std::cout << "random move in player!" << std::endl;
    cb.random_move();
}