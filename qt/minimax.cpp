#include "player.h"
#include "display.h"
#include "minimax.h"

Minimax::Minimax(bool isHuman_, Display *D_ptr_)
    : Player(isHuman_, D_ptr_)
    {}

void Minimax::think()
{
    // std::cout << "dummy move in player!" << std::endl;
    // D_ptr->cb.dummy_move();
    std::cout << "minimax move in minimax!" << std::endl;
    D_ptr->cb.random_move();
}


