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
    Minimax(bool isHuman_, Display *D_ptr_);

public:
    // bool isHuman;
    // Display *D_ptr;
    void think();

private:
    // int boardEval(int);

// public slots:
//     void think();
//     // void move(int,int,int,int,chessboard&);

// signals:
//     void send();
};

#endif // MINIMAX_H
