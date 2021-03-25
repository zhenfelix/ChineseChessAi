#ifndef MINIMAX_H
#define MINIMAX_H

#include "player.h"
#include <QObject>


class Minimax : public Player
{
    Q_OBJECT

public:
    Minimax(bool isHuman_, chessboard &cb_, int d_);//set search depth
    void think();

public:
    int d;//search depth

private:
    std::unordered_map<std::string,int> visited;
    void dfs(int depth, int alpha, int beta, int &best_score, std::pair<pos_type,pos_type> &best_move);
    //alpha beta pruning improves performance approximately 30x
    void dfs(int depth, int &best_score, std::pair<pos_type, pos_type> &best_move);
    void dfs_memo(int depth, int &best_score, std::pair<pos_type, pos_type> &best_move);
    //memo improves performance only less than 2x
    void negmax(int depth, int alpha, int beta, int &best_score, std::pair<pos_type, pos_type> &best_move);

// public slots:

// signals:
//     void send();
};

#endif // MINIMAX_H
