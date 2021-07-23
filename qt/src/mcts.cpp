#include "mcts.h"
#define INF 0x3f3f3f3f
#define NINF ~INF

#define C_UCB 1.4
// #define C_UCB 2

#define C_UCB_SCORE 1.4



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
    auto mv = calcMcts();
    // auto mv = calcMcts_score();
    // cb.move(mv);
    while (!cb.move(mv)){
        auto bestChild = (root->children).back();
        mv = bestChild->pmove;
        (root->children).pop_back();//memory leak
        delete bestChild;
        
    }
    // cb.move(calcMcts_score());
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

move_type Mcts::calcMcts()
{

    int color = cb.color;
    move_type null_move;
    cb.setShow(false);
//    std::vector<move_type> tmp = possbile_moves;
    if (root){
        root = moveRoot();
    }
    if (!root){
        std::vector<move_type> possbile_moves = cb.getMoves_quick();
        // std::cout << "root generate moves\n";
        root = new MctsNode(color, nullptr, null_move, possbile_moves);
    }
    
    cb.setShow(true);
    for (int i = 0; i < sim_cnt; i++)
    {
        cb_ptr = new chessboard(cb, true);
        cb_ptr->setShow(false);
        MctsNode* cur = select();
        int win_color = rollout();
//        if(cb_my.game_running)continue;
        backpropagate(cur,win_color);
        // std::cout << i << " similations finished\n";
        if(i%10000 == 0)std::cout << i <<" similations finished\n";
        delete cb_ptr;
    }
    root->show_children();
    MctsNode* child = chooseChild();
    // std::cout << "mcts choose move: " << child->pmove << std::endl;
    return child->pmove;
}

move_type Mcts::calcMcts_score()
{

    int color = cb.color;
    move_type null_move;
    cb.setShow(false);
    if (root)
    {
        root = moveRoot();
    }
    if (!root)
    {
        std::vector<move_type> possbile_moves = cb.getMoves_quick();
        root = new MctsNode(color, nullptr, null_move, possbile_moves);
    }

    cb.setShow(true);
    for (int i = 0; i < sim_cnt; i++)
    {
        cb_ptr = new chessboard(cb, true);
        cb_ptr->setShow(false);
        MctsNode *cur = select_score();
        int score = rollout_score();
        // std::cout << i << "simulation score: " << score << std::endl;
        backpropagate_score(cur, score);
        if (i % 10000 == 0)
            std::cout << i << " similations finished\n";
        delete cb_ptr;
    }
    root->show_children();
    MctsNode *child = chooseChild();
    // std::cout << "mcts choose move: " << child->pmove << std::endl;
    
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
            move_type nxt_move = cur->possible_moves.back();
            cur->possible_moves.pop_back();
            cb_ptr->move_quick(nxt_move);
            MctsNode* child = new MctsNode(cb_ptr->color,cur,nxt_move,cb_ptr->getMoves_quick());
            // std::cout << "node generate moves\n";
            cur->children.push_back(child);
            cur = child;
            break;
        }
        
    }
    return cur;

}

MctsNode *Mcts::select_score()
{
    MctsNode *cur = root;
    while (cb_ptr->game_running)
    {
        if (cur->possible_moves.empty())
        {
            cur = ucb_score(cur, C_UCB_SCORE);
            cb_ptr->move_quick(cur->pmove);
        }
        else
        {
            move_type nxt_move = cur->possible_moves.back();
            cur->possible_moves.pop_back();
            cb_ptr->move_quick(nxt_move);
            MctsNode *child = new MctsNode(cb_ptr->color, cur, nxt_move, cb_ptr->getMoves_quick());
            cur->children.push_back(child);
            cur = child;
            break;
        }
    }
    return cur;
}

MctsNode* Mcts::ucb(MctsNode* cur, double c_ucb)
{
    // MctsNode* bestChild = nullptr;
    // double bestScore = -1.0;
    // double curScore;
    // for(std::vector<MctsNode*>::iterator it = cur->children.begin(); it != cur->children.end(); it++)
    // {
    //     MctsNode *child = *it;
    //     curScore = (child->wins) * 1.0 / (child->n) + c_ucb * sqrt(log(cur->n) / (child->n));
    //     if(curScore > bestScore)
    //     {
    //         bestScore = curScore;
    //         bestChild = child;
    //     }
    // }
    MctsNode *bestChild = *std::max_element(cur->children.begin(), cur->children.end(), [&](MctsNode* a, MctsNode* b){
        double sa = (a->wins) * 1.0 / (a->n) + c_ucb * sqrt(log(cur->n) / (a->n));
        double sb = (b->wins) * 1.0 / (b->n) + c_ucb * sqrt(log(cur->n) / (b->n));
        return sa < sb;
    });
    return bestChild;
}

MctsNode *Mcts::ucb_score(MctsNode *cur, double c_ucb)
{
    // MctsNode *bestChild = nullptr;
    // double bestScore = -1.0;
    // double curScore;
    // for (std::vector<MctsNode *>::iterator it = cur->children.begin(); it != cur->children.end(); it++)
    // {
    //     MctsNode *child = *it;
    //     curScore = (child->score) * 1.0 / (child->n) + c_ucb * sqrt(log(cur->n) / (child->n));
    //     if (curScore > bestScore)
    //     {
    //         bestScore = curScore;
    //         bestChild = child;
    //     }
    // }
    MctsNode *bestChild = *std::max_element(cur->children.begin(), cur->children.end(), [&](MctsNode* a, MctsNode* b){
        double sa = (a->score) * 1.0 / (a->n) + c_ucb * sqrt(log(cur->n) / (a->n));
        double sb = (b->score) * 1.0 / (b->n) + c_ucb * sqrt(log(cur->n) / (b->n));
        return sa < sb;
    });
    return bestChild;
}

MctsNode *Mcts::chooseChild()
{
    MctsNode *bestChild = nullptr;
    int bestScore = -1;
    int curScore;

    // bestChild = *std::max_element(root->children.begin(), root->children.end(), [&](MctsNode* a, MctsNode* b){
    //     return a->n < b->n;
    // });
    std::sort(root->children.begin(), root->children.end(), [&](MctsNode* a, MctsNode* b){
        return a->n < b->n;
    });
    bestChild = (root->children).back();
    // (root->children).pop_back();//memory leak
    return bestChild;
}

int Mcts::rollout()
{
    for (int i = 0; i < MAXSTEP; i++)
    {
        if(!cb_ptr->game_running)break;
        // cb_ptr->random_move();

        // cb_ptr->sortMoves(candidates);
        // cb_ptr->move_quick(candidates[0]);

        auto candidates = cb_ptr->getMoves_quick();
        cb_ptr->move_quick(cb_ptr->greedyMove(candidates));
        // if(i > 0 && i%200 == 0)std::cout << i << "round rollout\n";
    }
    if(!cb_ptr->game_running)return -cb_ptr->color;
    if(cb_ptr->boardPosEval() > 0)return cb_ptr->color;
    return -cb_ptr->color;
}

int Mcts::rollout_score()
{
    for (int i = 0; i < MAXSTEP; i++)
    {
        if (!cb_ptr->game_running)
            break;
        // cb_ptr->random_move();
        auto candidates = cb_ptr->getMoves_quick();
        cb_ptr->move_quick(cb_ptr->greedyMove(candidates));
    }
    int score = (cb_ptr->boardPosEval() > 0 ? 1 : -1);
    if (!cb_ptr->game_running)
        score *= 1;
    score = cb_ptr->color * score;
    return score;
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

void Mcts::backpropagate_score(MctsNode *cur, int score)
{
    while (cur)
    {
        cur->n++;
        cur->score += -cur->color*score;
        cur = cur->parent;
    }
}