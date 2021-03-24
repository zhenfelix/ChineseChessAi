#ifndef PLAYER_H
#define PLAYER_H

#include "stone.h"
#include "gameEngine.h"
#include <QObject>



class Player : public QObject
{
    Q_OBJECT

public:
    Player(bool isHuman_, chessboard &cb_);//
    virtual void think();

public:
    bool isHuman;
    chessboard &cb;
    

// public slots:


// signals:
//     void send();
};

#endif // PLAYER_H
