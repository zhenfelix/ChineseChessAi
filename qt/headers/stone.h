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
#include <set>
#include <unordered_map>
#include <stack>
#include <utility>
#include <time.h>
#include <unistd.h>
#include <ctime>
#include <algorithm>
#include <string>

// #include "gameEngine.h"

typedef std::pair<int,int> pos_type;

struct Timer
{
    clock_t start, end;
    double duration;
    std::string msg;

    Timer(std::string msg_="Timer took ")
        :msg(msg_)
    {
        start = clock();
    }
    ~Timer()
    {
        end = clock();
        duration = (end - start) / (double)CLOCKS_PER_SEC;
        std::cout << msg << duration << "s" << std::endl;
    }
};

//存储结构：chess类是基类，派生类是各种棋子，在chessboard类中用chess的指针调用各个棋子
class chessboard;
class Stone
{
private:
    int id; //等级
    int row, col;
    int vertical;//stone's host vertical position
public:
    Stone(int i) : id(i) {}
    Stone(int i, int r, int c, int v): id(i), row(r), col(c), vertical(v) {}
    Stone(const Stone &stone) : id(stone.id), row(stone.row), col(stone.col), vertical(stone.vertical) {}
    virtual Stone* Clone() = 0;
    int get() { return id; }
    void getPos(int &r, int &c){ r=row; c=col; return;}
    int getVer(){ return vertical; }
    void setPos(int r, int c){ row=r; col=c; return;}
    virtual bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy) = 0;
    // virtual bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy){}
    virtual ~Stone(){}; //虚析构
};

class horse : public Stone
{
public:
    horse(int,int,int,int);
    horse(const horse& derived): Stone(derived){}
    horse* Clone(){ return new horse(*this); }
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

class soldier : public Stone
{
public:
    soldier(int,int,int,int);
    soldier(const soldier &derived) : Stone(derived) {}
    soldier *Clone() { return new soldier(*this); }
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

class general : public Stone
{
public:
    general(int,int,int,int);
    general(const general &derived) : Stone(derived) {}
    general *Clone() { return new general(*this); }
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

class elephant : public Stone
{
public:
    elephant(int,int,int,int);
    elephant(const elephant &derived) : Stone(derived) {}
    elephant *Clone() { return new elephant(*this); }
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

class cannon : public Stone
{
public:
    cannon(int,int,int,int);
    cannon(const cannon &derived) : Stone(derived) {}
    cannon *Clone() { return new cannon(*this); }
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

class guard : public Stone
{
public:
    guard(int,int,int,int);
    guard(const guard &derived) : Stone(derived) {}
    guard *Clone() { return new guard(*this); }
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

class rook : public Stone
{
public:
    rook(int,int,int,int);
    rook(const rook &derived) : Stone(derived) {}
    rook *Clone() { return new rook(*this); }
    bool judge_move(chessboard &cb, int startx, int starty, int aimx, int aimy);
};

#endif // STONE_H
