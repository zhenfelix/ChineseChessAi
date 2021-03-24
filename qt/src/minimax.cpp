#include "minimax.h"

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
     std::pair<std::pair<int, int>, std::pair<int, int>> best_move;
     cb.setShow(false);//silence board show while computer thinking
     dfs(d,INT_MIN,INT_MAX,best_score,best_move);
    //  dfs(d, best_score, best_move);
    //  dfs_memo(d, best_score, best_move);
     cb.setShow(true);
     auto &[start_xy, aim_xy] = best_move;
     auto &[startx, starty] = start_xy;
     auto &[aimx, aimy] = aim_xy;
     if(cb.color == MAXIMIZER_COLOR)
     {
         std::cout << "computer with color: " << cb.color << " is a maximizer" << std::endl;
     }
     else
     {
         std::cout << "computer with color: " << cb.color << " is a minimizer" << std::endl;
     }
     
     std::cout << "computer with color: " << cb.color << " see best score: " << best_score << std::endl;
     duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
     std::cout << "it took computer " << duration << " seconds to think\n\n\n";
     cb.move(startx, starty, aimx, aimy);
}

void Minimax::dfs(int depth, int alpha, int beta, int &best_score, std::pair<std::pair<int, int>, std::pair<int, int>> &best_move)
{
    std::string board_hash = cb.boardHash();

    if(depth == 0 || !cb.game_running)//fix bug, ai should react to general in check
    //todo avoid cyclic check
    {
        best_score = cb.boardEval(MAXIMIZER_COLOR);
        return;
    }
    
    if (cb.color == MAXIMIZER_COLOR) best_score = INT_MIN;
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
         if (alpha >= beta)
             break;
     }
     return;
}

void Minimax::dfs(int depth, int &best_score, std::pair<std::pair<int, int>, std::pair<int, int>> &best_move)
{
    // std::string board_hash = cb.boardHash();
    // if (visited.find(board_hash) != visited.end())
    // {
    //     best_score = visited[board_hash];
    //     return;
    // }
    if (depth == 0 || !cb.game_running) //fix bug, ai should react to general in check
    //todo avoid cyclic check
    //enable memorization to speedup
    {
        best_score = cb.boardEval(MAXIMIZER_COLOR);
        // visited.insert({board_hash, best_score});
        return;
    }

    if (cb.color == MAXIMIZER_COLOR)
        best_score = INT_MIN;
    else
        best_score = INT_MAX;
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
        dfs(depth - 1, current_score, current_move);
        cb.undo();
        if (cb.color == MAXIMIZER_COLOR)
        {
            if (current_score > best_score)
            {
                best_score = current_score;
                //                 best_move = candidate;
                best_move = {start_xy, aim_xy};
            }
        }
        else
        {
            if (current_score < best_score)
            {
                best_score = current_score;
                //                 best_move = candidate;
                best_move = {start_xy, aim_xy};
            }
        }
    }
    // visited.insert({board_hash, best_score});
    return;
}

void Minimax::dfs_memo(int depth, int &best_score, std::pair<std::pair<int, int>, std::pair<int, int>> &best_move)
{
    std::string board_hash = cb.boardHash();
    char ch;
    ch = '0' + depth;
    board_hash += ch;
    if (visited.find(board_hash) != visited.end())
    {
        best_score = visited[board_hash];
        return;
    }
    if (depth == 0 || !cb.game_running) //fix bug, ai should react to general in check
    //todo avoid cyclic check
    //enable memorization to speedup
    {
        best_score = cb.boardEval(MAXIMIZER_COLOR);
        visited.insert({board_hash, best_score});
        return;
    }

    if (cb.color == MAXIMIZER_COLOR)
        best_score = INT_MIN;
    else
        best_score = INT_MAX;
    auto candidates = cb.getMoves();
    for (auto &[start_xy, aim_xy] : candidates)
    {
        int current_score;
        std::pair<std::pair<int, int>, std::pair<int, int>> current_move;
        auto &[startx, starty] = start_xy;
        auto &[aimx, aimy] = aim_xy;

        cb.move(startx, starty, aimx, aimy);
        dfs_memo(depth - 1, current_score, current_move);
        cb.undo();
        if (cb.color == MAXIMIZER_COLOR)
        {
            if (current_score > best_score)
            {
                best_score = current_score;
                //                 best_move = candidate;
                best_move = {start_xy, aim_xy};
            }
        }
        else
        {
            if (current_score < best_score)
            {
                best_score = current_score;
                //                 best_move = candidate;
                best_move = {start_xy, aim_xy};
            }
        }
    }
    visited.insert({board_hash, best_score});
    return;
}