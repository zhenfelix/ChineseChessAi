#ifndef MINIMAX_H
#define MINIMAX_H

#include "stone.h"
#include "gameEngine.h"
#include "player.h"
// #include "display.h"
#include <QObject>
class Minimax : public Player
{
    Q_OBJECT

public:
    Minimax(bool isHuman_, Display *D_ptr_, int maximizer_color_);
    void think();

public:
    int maximizer_color;

private:
    void dfs(int depth, int alpha, int beta, int &best_score, std::pair<std::pair<int,int>,std::pair<int,int>> &best_move);

// public slots:
//     void think();
//     // void move(int,int,int,int,chessboard&);

// signals:
//     void send();
};

#endif // MINIMAX_H
