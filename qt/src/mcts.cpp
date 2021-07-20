#include "mcts.h"
#define INF 0x3f3f3f3f
#define NINF ~INF

#define C_UCB 1.4

Mcts::Mcts(bool isHuman_, chessboard &cb_, int sim_cnt_/*=10000*/) //similation
    : Player(isHuman_, cb_), sim_cnt(sim_cnt_), root(nullptr)
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
    Timer timer("it took computer mcts ");
    cb.move(calcMcts());
    // std::cout << "total nodes from current root: " << count(root) << std::endl;
    // std::cout << "total size of nodes from current root: " << count(root) * sizeof(MctsNode) << std::endl;
    root = moveRoot();
}

int Mcts::count(MctsNode* cur){
    if (!cur)
        return 0;
    int cnt = 1;
    for (auto it = cur->children.begin(); it != cur->children.end(); it++){
        auto nxt = *it;
        cnt += count(nxt);
    }
    return cnt;
}

std::pair<pos_type,pos_type> Mcts::calcMcts()
{

    int color = cb.color;
    std::pair<pos_type,pos_type> null_move;
    cb.setShow(false);
//    std::vector<std::pair<pos_type,pos_type>> tmp = possbile_moves;
    if (root){
        root = moveRoot();
    }
    if (!root){
        std::vector<std::pair<pos_type, pos_type>> possbile_moves = cb.getMoves_quick();
        root = new MctsNode(color, nullptr, null_move, possbile_moves);
    }
    
    cb.setShow(true);
    for (int i = 0; i < sim_cnt; i++)
    {
        cb_ptr = new chessboard(cb);
        cb_ptr->setShow(false);
        MctsNode* cur = select();
        int win_color = rollout();
//        if(cb_my.game_running)continue;
        backpropagate(cur,win_color);
        if(i%5000 == 0)std::cout << i <<" similations finished\n";
        delete cb_ptr;
    }
    MctsNode* child = chooseChild(root);
    std::cout << "mcts choose move: " << child->pmove.first.first << ", " << child->pmove.first.second << ", " << child->pmove.second.first << ", " << child->pmove.second.second << std::endl;
    return child->pmove;
}

MctsNode* Mcts::moveRoot(){
    auto last_move = cb.getLastMove();
    MctsNode* candidate = nullptr;
    // std::vector<MctsNode*> tmp_children;
    // for (auto it = root->children.begin(); it != root->children.end(); it++){
    //     MctsNode* child = *it;
    //     if (child->pmove == last_move){
    //         std::cout << "reused monte carlo tree! find the next round root!\n";
    //         candidate = child;
    //     }
    //     else{
    //         tmp_children.push_back(child);
    //     }
    // }
    // if (candidate){
    //     candidate->parent = nullptr;
    //     std::swap(root->children, tmp_children);
    // }
    delete root;
    return candidate;
}


MctsNode* Mcts::select()
{
    MctsNode* cur = root;
    while (cb_ptr->game_running)
    {
        if(cur->possible_moves.empty())
        {
            cur = ucb(cur,C_UCB);
            cb_ptr->move_quick(cur->pmove);
        }
        else
        {
            std::pair<pos_type,pos_type> nxt_move = cur->possible_moves.back();
            cur->possible_moves.pop_back();
            cb_ptr->move_quick(nxt_move);
            MctsNode* child = new MctsNode(cb_ptr->color,cur,nxt_move,cb_ptr->getMoves_quick());
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
        if(!cb_ptr->game_running)break;
        // cb_ptr->random_move();
        auto candidates = cb_ptr->getMoves_quick();
        // cb_ptr->sortMoves(candidates);
        // cb_ptr->move_quick(candidates[0]);
        cb_ptr->move_quick(cb_ptr->greedyMove(candidates));
        // if(i > 0 && i%200 == 0)std::cout << i << "round rollout\n";
    }
    if(!cb_ptr->game_running)return -cb_ptr->color;
    if(cb_ptr->boardPosEval() > 0)return cb_ptr->color;
    return -cb_ptr->color;
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
