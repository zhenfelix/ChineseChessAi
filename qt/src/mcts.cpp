#include "mcts.h"
#define INF 0x3f3f3f3f
#define NINF ~INF



Mcts::Mcts(bool isHuman_, chessboard &cb_, int sim_cnt_ = 1000) //similation
    : Player(isHuman_, cb_), sim_cnt(sim_cnt_)
{}

void Mcts::think()
{
    std::cout << "begin mcts simulation in a virtual board\n";
    chessboard cb_my = cb;
    for (int i = 0; i < MAXSTEP; i++)
    {
        if (!cb_my.game_running)
        {
            std::cout << "game over! at max step: " << i << std::endl;
            break;
        }
        cb_my.random_move();
    }
    std::cout << "finish mcts simulation in a virtual board\n";
    std::cout << "random move in player!" << std::endl;
    cb.random_move();
}
