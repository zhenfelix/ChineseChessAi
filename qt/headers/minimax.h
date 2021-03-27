#ifndef MINIMAX_H
#define MINIMAX_H

#include "player.h"
#include <QObject>

enum tt_state
{
    EXACT,
    LOWERBOUND,
    UPPERBOUND
};

struct tt_entry
{
    tt_state flag;
    int depth;
    int value;
    std::pair<pos_type, pos_type> best_move;
};


class Minimax : public Player
{
    Q_OBJECT

public:
    Minimax(bool isHuman_, chessboard &cb_, int d_);//set search depth
    void think();

public:
    int d;//search depth

private:
    // std::unordered_map<std::string,tt_entry> visited;
    std::unordered_map<std::string, tt_entry> visited;

private:
    void dfs(int depth, int alpha, int beta, int &best_score, std::pair<pos_type,pos_type> &best_move);
    //alpha beta pruning improves performance ~30x, compared to bruteforce
    void negmax(int depth, int alpha, int beta, int &best_score, std::pair<pos_type, pos_type> &best_move);
    //negmax implementation has similar performance as minimax
    void negmax_memo(int depth, int alpha, int beta, int &best_score, std::pair<pos_type, pos_type> &best_move);
    //memorization only improves performance ~1x, neligible

    void quiesceneSearch(bool is_quiescene, int depth, int alpha, int beta, int &best_score, std::pair<pos_type, pos_type> &best_move);

    // public slots:

    // signals:
    //     void send();
};

#endif // MINIMAX_H
