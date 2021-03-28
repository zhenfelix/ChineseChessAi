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

    //  pvs(d, NINF, INF, best_score, best_move);
    //   quiesceneSearch(false, d, NINF, INF, best_score, best_move);
      negmax(d, NINF, INF, best_score, best_move);
     //  negmax_memo(d, NINF, INF, best_score, best_move);
     std::cout << "computer with color: " << cb.color << " see its best score: " << best_score << std::endl;

     //unsolved bug, negmax_memo and negmax have different behavior while playing with a weaker self??? should be the same?? but the same while playing with a random move player
     //bug fixed, 2 bugs, 1.map insert vs [] operator difference; 2. tt_entry should also memorize best_move apart from best_score!
     //new bug, cannot see upcoming checkmate
     
    //  dfs(d,NINF,INF,best_score,best_move);
    //  if (cb.color == MAXIMIZER_COLOR)
    //  {
    //      std::cout << "computer with color: " << cb.color << " is a maximizer" << std::endl;
    //  }
    //  else
    //  {
    //      std::cout << "computer with color: " << cb.color << " is a minimizer" << std::endl;
    //  }
    //  std::cout << "computer with color: " << cb.color << " see best score: " << best_score << std::endl;

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
        if (d - depth <= 2)
            best_score *= 2; //you have to handle immediate check
        return;
    }
    
    if (cb.color == MAXIMIZER_COLOR) best_score = NINF;
    else best_score = INF;
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
        // best_score = cb.boardEvalNegMax();
        best_score = cb.boardPosEval();
        if (d - depth <= 2)
            best_score *= 2; //you have to handle immediate check
        return;
    }

    best_score = NINF;

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

void Minimax::negmax_memo(int depth, int alpha, int beta, int &best_score, std::pair<pos_type, pos_type> &best_move)
{
    std::string board_hash = cb.boardHash();
    int alphaOrig = alpha;
    if(visited.find(board_hash) != visited.end())
    {
        tt_entry tte = visited[board_hash];
        if (tte.depth >= depth)//important !!!
        {
            if (tte.flag == EXACT)
            {
                best_score = tte.value;
                best_move = tte.best_move;//important !!! fix bug !!!
                return;
            }
            else if (tte.flag == LOWERBOUND)
            {
                alpha = std::max(alpha, tte.value);
            }
            else if (tte.flag == UPPERBOUND)
            {
                beta = std::min(beta, tte.value);
            }
            if (alpha >= beta)
            {
                best_score = tte.value;
                best_move = tte.best_move; //important !!! fix bug !!!
                return;
            }
        }
        
    }

    if (depth == 0 || !cb.game_running)
    {
        // best_score = cb.boardEvalNegMax();
        best_score = cb.boardPosEval();
        if (d - depth <= 2)
            best_score *= 2; //you have to handle immediate check
        return;
    }

    best_score = NINF;

    auto candidates = cb.getMoves();
    cb.sortMoves(candidates);

    for (auto &[start_xy, aim_xy] : candidates)
    {
        int current_score;
        std::pair<pos_type, pos_type> current_move;
        auto &[startx, starty] = start_xy;
        auto &[aimx, aimy] = aim_xy;

        cb.move(startx, starty, aimx, aimy);
        negmax_memo(depth - 1, -beta, -alpha, current_score, current_move);
        cb.undo();
        current_score = -current_score;
        if (current_score > best_score)
        {
            best_score = current_score;
            best_move = {start_xy, aim_xy};
        }
        alpha = std::max(alpha, best_score);
        if (alpha >= beta) 
            break;
    }
    tt_entry tte;
    tte.depth = depth;
    tte.value = best_score;
    tte.best_move = best_move; //important !!! fix bug !!!
    if (best_score <= alphaOrig)
    {
        tte.flag = UPPERBOUND;
    }
    else if(best_score >= beta)
    {
        tte.flag = LOWERBOUND;
    }
    else
    {
        tte.flag = EXACT;
    }
    
    // visited.insert({board_hash,tte});//Because std::map does not allow for duplicates if there is an existing element it will not insert anything.
    visited[board_hash] = tte;
    return;
}

void Minimax::quiesceneSearch(bool is_quiescene, int depth, int alpha, int beta, int &best_score, std::pair<pos_type, pos_type> &best_move)
{
    // std::string board_hash = cb.boardHash();

    if (is_quiescene || depth == 0 || !cb.game_running)
    {
        // best_score = cb.boardEvalNegMax();
        best_score = cb.boardPosEval();
        if (!is_quiescene && d - depth <= 2)
            best_score *= 2; //you have to handle immediate check
        if (!cb.game_running)
            return;
        if (is_quiescene)
        {
            if (!cb.isVolatile() || depth == 0) return;
        }
        else
        {
            if(!cb.isVolatile()) return;
            quiesceneSearch(true,4,alpha,beta,best_score,best_move);//quiescene search depth!
            return;
        }
        
    }

    best_score = NINF;

    auto candidates = cb.getMoves();
    cb.sortMoves(candidates);

    for (auto &[start_xy, aim_xy] : candidates)
    {
        int current_score;
        std::pair<pos_type, pos_type> current_move;
        auto &[startx, starty] = start_xy;
        auto &[aimx, aimy] = aim_xy;

        cb.move(startx, starty, aimx, aimy);
        quiesceneSearch(is_quiescene, depth - 1, -beta, -alpha, current_score, current_move);
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

void Minimax::pvs(int depth, int alpha, int beta, int &best_score, std::pair<pos_type, pos_type> &best_move)
{
    // std::string board_hash = cb.boardHash();

    if (depth == 0 || !cb.game_running)
    {
        // best_score = cb.boardEvalNegMax();
        best_score = cb.boardPosEval();
        if (d - depth <= 2)
            best_score *= 2; //you have to handle immediate check
        return;
    }

    best_score = NINF;

    auto candidates = cb.getMoves();
    cb.sortMoves(candidates);
    bool first_move = true;
    for (auto &[start_xy, aim_xy] : candidates)
    {
        int current_score;
        std::pair<pos_type, pos_type> current_move;
        auto &[startx, starty] = start_xy;
        auto &[aimx, aimy] = aim_xy;

        
        if(first_move)
        {
            cb.move(startx, starty, aimx, aimy);
            pvs(depth - 1, -beta, -alpha, current_score, current_move);
            cb.undo();
            current_score = -current_score;
            first_move = false;
        }
        else
        {
            cb.move(startx, starty, aimx, aimy);
            pvs(depth - 1, -alpha-1, -alpha, current_score, current_move);
            cb.undo();
            current_score = -current_score;
            if (alpha < current_score && current_score < beta)
            {
                cb.move(startx, starty, aimx, aimy);
                pvs(depth - 1, -beta, -current_score, current_score, current_move);
                cb.undo();
                current_score = -current_score;
            }
        }
        
        
        
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