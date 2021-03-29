#ifndef MCTS_H
#define MCTS_H

#include "player.h"
#include <QObject>

#define MAXSTEP 100

class Mcts : public Player
{
    Q_OBJECT

public:
    Mcts(bool isHuman_, chessboard &cb_, int sim_cnt_);//set simlulation count
    void think();

public:
    

private:
    int sim_cnt; //simlulation count

private:
    
    // public slots:

    // signals:
    //     void send();
};

#endif // MCTS_H
