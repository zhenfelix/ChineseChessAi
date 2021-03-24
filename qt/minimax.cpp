#include "player.h"
#include "display.h"
#include "minimax.h"

Minimax::Minimax(bool isHuman_, Display *D_ptr_, int maximizer_color_=-1 )
    : Player(isHuman_, D_ptr_), maximizer_color(maximizer_color_)
    {}

void Minimax::think()
{
    // std::cout << "dummy move in player!" << std::endl;
    // D_ptr->cb.dummy_move();
    std::cout << "minimax move in minimax! wait for computer think..." << std::endl;
    // D_ptr->cb.random_move();


     int best_score;
     std::pair<std::pair<int, int>, std::pair<int, int>> best_move;
     chessboard &cb = D_ptr->cb;
     cb.setShow(false);
     dfs(4,INT_MIN,INT_MAX,best_score,best_move);//depth setting
     cb.setShow(true);
     auto &[start_xy, aim_xy] = best_move;
     auto &[startx, starty] = start_xy;
     auto &[aimx, aimy] = aim_xy;
     std::cout << "computer see best score: " << best_score << std::endl;
     cb.move(startx, starty, aimx, aimy);
}

void Minimax::dfs(int depth, int alpha, int beta, int &best_score, std::pair<std::pair<int, int>, std::pair<int, int>> &best_move)
{
    chessboard &cb = D_ptr->cb;
    if(depth == 0 || !cb.game_running)//fix bug, ai should react to general in check
    {
        best_score = cb.boardEval(maximizer_color);
        return;
    }
    
    if (cb.color == maximizer_color) best_score = INT_MIN;
    else best_score = INT_MAX;
     auto candidates = cb.getMoves();
     for (auto &[start_xy, aim_xy] : candidates)
//     for (auto &candidate: candidates)
     {
         int current_score;
         std::pair<std::pair<int, int>, std::pair<int, int>> current_move;
//         auto [start_xy, aim_xy] = candidate;
         auto &[startx, starty] = start_xy;
         auto &[aimx, aimy] = aim_xy;

         cb.move(startx, starty, aimx, aimy);
         dfs(depth-1,alpha,beta,current_score,current_move);
         cb.undo();
         if (cb.color == maximizer_color)
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
         if (alpha >= beta)
             return;
     }
    return;
}
