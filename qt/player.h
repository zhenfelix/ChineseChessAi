#ifndef PLAYER_H
#define PLAYER_H

#include "stone.h"
#include "gameEngine.h"
// #include "display.h"
#include <QObject>
class Display;
class Player : public QObject
{
    Q_OBJECT

public:
    Player(bool isHuman_, Display *D_ptr_);
    // void think();
    // void move(int,int,int,int,chessboard&);
    // void send();

public:
    bool isHuman;
    Display *D_ptr;
    virtual void think();

// public slots:
//     void think();
//     // void move(int,int,int,int,chessboard&);

// signals:
//     void send();
};

#endif // PLAYER_H
