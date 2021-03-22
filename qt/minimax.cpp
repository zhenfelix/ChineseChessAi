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

// int Minimax::boardEval(chessboard &cb)
// {
//     int score = 0;
//     for (int r = 0; r < 10; r++)
//     {
//         for (int col = 0; col < 9; col++)
//         {
//             if(cb.c[row][col])
//         }
        
//     }
    
// }