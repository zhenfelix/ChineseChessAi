#include "mcts.h"
#define INF 0x3f3f3f3f
#define NINF ~INF

#define C_UCB 1.4

Mcts::Mcts(bool isHuman_, chessboard &cb_, int sim_cnt_/*=10000*/) //similation
    : Player(isHuman_, cb_), sim_cnt(sim_cnt_), cb_my(false)
{}

// void Mcts::think()
// {
//     std::cout << "begin mcts simulation in a virtual board\n";
//     chessboard cb_my = cb;
//     for (int i = 0; i < MAXSTEP; i++)
//     {
//         if (!cb_my.game_running)
//         {
//             std::cout << "game over! at max step: " << i << std::endl;
//             break;
//         }
//         cb_my.random_move();
//     }
//     std::cout << "finish mcts simulation in a virtual board\n";
//     std::cout << "random move in player!" << std::endl;
//     cb.random_move();
// }

void Mcts::think()
{
    cb.move(calcMcts());
}

std::pair<pos_type,pos_type> Mcts::calcMcts()
{

    int color = cb.color;
    std::pair<pos_type,pos_type> null_move;
    cb.setShow(false);
    std::vector<std::pair<pos_type,pos_type>> possbile_moves = cb.getMoves();
//    std::vector<std::pair<pos_type,pos_type>> tmp = possbile_moves;
    root = new MctsNode(color, nullptr, null_move, possbile_moves);
    cb.setShow(true);
    for (int i = 0; i < sim_cnt; i++)
    {
        cb_my = cb;
        MctsNode* cur = select();
        int win_color = rollout();
//        if(cb_my.game_running)continue;
        backpropagate(cur,win_color);
        if(i%100 == 0)std::cout << i <<" similations finished\n";
    }
    MctsNode* child = chooseChild(root);
    return child->pmove;
}

MctsNode* Mcts::select()
{
    MctsNode* cur = root;
    while (cb_my.game_running)
    {
        if(cur->possible_moves.empty())
        {
            cur = ucb(cur,C_UCB);
            cb_my.move(cur->pmove);
        }
        else
        {
            std::pair<pos_type,pos_type> nxt_move = cur->possible_moves.back();
            cur->possible_moves.pop_back();
            cb_my.move(nxt_move);
            MctsNode* child = new MctsNode(cb_my.color,cur,nxt_move,cb_my.getMoves());
            cur->children.push_back(child);
            cur = child;
            break;
        }
        
    }
    return cur;

}

MctsNode* Mcts::ucb(MctsNode* cur, double c_ucb)
{
    MctsNode* bestChild = nullptr;
    double bestScore = -1.0;
    double curScore;
    for(std::vector<MctsNode*>::iterator it = cur->children.begin(); it != cur->children.end(); it++)
    {
        MctsNode *child = *it;
        curScore = (child->wins) * 1.0 / (child->n) + c_ucb * sqrt(log(cur->n) / (child->n));
        if(curScore > bestScore)
        {
            bestScore = curScore;
            bestChild = child;
        }
    }
    return bestChild;
}

MctsNode *Mcts::chooseChild(MctsNode *cur)
{
    MctsNode *bestChild = nullptr;
    int bestScore = -1;
    int curScore;
    for (std::vector<MctsNode *>::iterator it = cur->children.begin(); it != cur->children.end(); it++)
    {
        MctsNode *child = *it;
        curScore = (child->n);
        if (curScore > bestScore)
        {
            bestScore = curScore;
            bestChild = child;
        }
    }
    return bestChild;
}

int Mcts::rollout()
{
    for (int i = 0; i < MAXSTEP; i++)
    {
        if(!cb_my.game_running)break;
        cb_my.random_move();
        // if(i > 0 && i%200 == 0)std::cout << i << "round rollout\n";
    }
    if(!cb_my.game_running)return -cb_my.color;
    if(cb_my.boardPosEval() > 0)return cb_my.color;
    return -cb_my.color;
}

void Mcts::backpropagate(MctsNode* cur, int win_color)
{
    while (cur)
    {
        cur->n++;
        if (win_color == -(cur->color)) cur->wins++;
        cur = cur->parent;
    }
    
}
