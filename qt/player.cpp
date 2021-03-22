#include "player.h"
#include "display.h"

Player::Player(bool isHuman_, Display *D_ptr_)
    : isHuman(isHuman_), D_ptr(D_ptr_)
    {
        // connect(this, SIGNAL(send()), D_ptr, SLOT(turnOver()));
    }

void Player::think()
{
    // std::cout << "dummy move in player!" << std::endl;
    // D_ptr->cb.dummy_move();
    std::cout << "random move in player!" << std::endl;
    D_ptr->cb.random_move();
}