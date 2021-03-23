#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "stone.h"

class chessboard : public QObject
{
    Q_OBJECT
private:
    Stone *c[10][9];                                                                                                  //用指针调用各个棋子，实现多态
    // static char chessword[15][4] = {"帥", "相", "炮", "仕", "車", "馬", "兵", "", "卒", "马", "车", "士", "砲", "象", "将"}; //名字

private:
    void placeStone(int color_, int vertical_);

public:
    // chessboard();
    chessboard(bool);
    // chessboard(chessboard&);
    void init();
    Stone *get(int x, int y) { return c[x][y]; }
    int getVertical(){ return vertical; }
    std::vector<Stone*> getStones();
    int getid(int x, int y)
    {
        if (c[x][y] != NULL)
            return c[x][y]->get();
        return 0;
    }
    void setShow(bool is_show_){ is_show = is_show_; }
    void show();
    void play();
    bool move(int startx, int starty, int aimx, int aimy);
    // void moveBack();
    void dummy_move();
    void random_move();
    int boardEval(int);
    ~chessboard();   //析构函数

public:
    static char chessword[15][4];
    static int stonevalue[15];
    int color; //current player color, red: -1
    int vertical;       // first hand vertical position, 1: up, -1 down
    bool game_running;  //判断结束
    bool is_show;       //computer self played chessboard will not be shown

    
    std::stack<std::pair<std::pair<int,int>,std::pair<int,int>>> records;
    std::map<int,Stone*> capturedStones;

signals:
    void update();


public slots:
    void undo();
    void flip();
};

#endif // GAMEENGINE_H
