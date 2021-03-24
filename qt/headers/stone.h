#ifndef STONE_H
#define STONE_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QString>
#include <QPushButton>

#include <iostream>
#include <memory.h>
#include <math.h>
#include <vector>
#include <queue>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <utility>
#include <time.h>
#include <unistd.h>
#include <ctime>
#include <algorithm>

// #include "gameEngine.h"


//存储结构：chess类是基类，派生类是各种棋子，在chessboard类中用chess的指针调用各个棋子
class chessboard;
class Stone
{
private:
    int id; //等级
    int row, col;
public:
    Stone(int i) : id(i) {}
    Stone(int i, int r, int c): id(i), row(r), col(c) {}
    int get() { return id; }
    void getPos(int &r, int &c){ r=row; c=col; return;}
    void setPos(int r, int c){ row=r; col=c; return;}
    virtual bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy) = 0;
    virtual ~Stone(){}; //虚析构
};

class horse : public Stone
{
public:
    horse(int,int,int);
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

class soldier : public Stone
{
public:
    soldier(int,int,int);
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

class general : public Stone
{
public:
    general(int,int,int);
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

class elephant : public Stone
{
public:
    elephant(int,int,int);
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

class cannon : public Stone
{
public:
    cannon(int,int,int);
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

class guard : public Stone
{
public:
    guard(int,int,int);
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

class rook : public Stone
{
public:
    rook(int,int,int);
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

#endif // STONE_H
