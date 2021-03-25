#include "minimax.h"
#define INF 0x3f3f3f3f
#define NINF ~INF

Minimax::Minimax(bool isHuman_, chessboard &cb_, int d_ = 5) //depth setting
    : Player(isHuman_, cb_), d(d_)
{}

void Minimax::think()
{
    // std::cout << "dummy move in player!" << std::endl;
    // cb.dummy_move();
    std::clock_t start;
    double duration;
    start = std::clock();
    std::cout << "minimax move in minimax! wait for computer to think..." << std::endl;

     int best_score;
     std::pair<pos_type, pos_type> best_move;
     cb.setShow(false);//silence board show while computer thinking

    //  negmax(d, INT_MIN, INT_MAX, best_score, best_move);//dangerous bug!!! -INT_MIN still INT_MIN!!! overflow!!!
     negmax(d, NINF, INF, best_score, best_move);
     std::cout << "computer with color: " << cb.color << " see its best score: " << best_score << std::endl;

     // dfs(d,NINF,INF,best_score,best_move);
     // if (cb.color == MAXIMIZER_COLOR)
     // {
     //     std::cout << "computer with color: " << cb.color << " is a maximizer" << std::endl;
     // }
     // else
     // {
     //     std::cout << "computer with color: " << cb.color << " is a minimizer" << std::endl;
     // }
     // std::cout << "computer with color: " << cb.color << " see best score: " << best_score << std::endl;

     cb.setShow(true);
     duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
     std::cout << "it took computer " << duration << " seconds to think\n\n\n";

     auto &[start_xy, aim_xy] = best_move;
     auto &[startx, starty] = start_xy;
     auto &[aimx, aimy] = aim_xy;
     cb.move(startx, starty, aimx, aimy);
}

void Minimax::dfs(int depth, int alpha, int beta, int &best_score, std::pair<pos_type, pos_type> &best_move)
{
    // std::string board_hash = cb.boardHash();

    if(depth == 0 || !cb.game_running)//fix bug, ai should react to general in check
    //todo avoid cyclic check
    {
        best_score = cb.boardEval(MAXIMIZER_COLOR);
        return;
    }
    
    if (cb.color == MAXIMIZER_COLOR) best_score = INT_MIN;
    else best_score = INT_MAX;
    auto candidates = cb.getMoves();
    cb.sortMoves(candidates);
     
     for (auto &[start_xy, aim_xy] : candidates)
//     for (auto &candidate: candidates)
     {
         int current_score;
         std::pair<pos_type, pos_type> current_move;
//         auto [start_xy, aim_xy] = candidate;
         auto &[startx, starty] = start_xy;
         auto &[aimx, aimy] = aim_xy;

         cb.move(startx, starty, aimx, aimy);
         dfs(depth-1,alpha,beta,current_score,current_move);
         cb.undo();
         if (cb.color == MAXIMIZER_COLOR)
         {
             if (current_score > best_score)
             {
                 best_score = current_score;
//                 best_move = candidate;
                 best_move = {start_xy, aim_xy};
             }
             alpha = std::max(alpha, current_score);
         }
         else
         {
             if (current_score < best_score)
             {
                 best_score = current_score;
//                 best_move = candidate;
                 best_move = {start_xy, aim_xy};
             }
             beta = std::min(beta, current_score);
         }
         if (alpha >= beta) //brute force, comment out this line
             break;
     }
     return;
}



void Minimax::negmax(int depth, int alpha, int beta, int &best_score, std::pair<pos_type, pos_type> &best_move)
{
    // std::string board_hash = cb.boardHash();

    if (depth == 0 || !cb.game_running) 
    {
        best_score = cb.boardEvalNegMax();
        return;
    }

    best_score = INT_MIN;

    auto candidates = cb.getMoves();
    cb.sortMoves(candidates);

    for (auto &[start_xy, aim_xy] : candidates)
    {
        int current_score;
        std::pair<pos_type, pos_type> current_move;
        auto &[startx, starty] = start_xy;
        auto &[aimx, aimy] = aim_xy;

        cb.move(startx, starty, aimx, aimy);
        negmax(depth - 1, -beta, -alpha, current_score, current_move);
        cb.undo();
        current_score = -current_score;
        if (current_score > best_score)
        {
            best_score = current_score;
            best_move = {start_xy, aim_xy};
        }
        alpha = std::max(alpha, best_score);
        if (alpha >= beta) // bug in negmax with alpha-beta, solved -INT_MIN overflow
            break;
    }
    return;
}