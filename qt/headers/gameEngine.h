#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "stone.h"

#define MAXIMIZER_COLOR -1 //default maximizer color is -1

class chessboard : public QObject
{
    Q_OBJECT

public:
    // chessboard();
    chessboard(bool);
    // chessboard(chessboard&);
    void init();


    Stone *get(int x, int y) { return c[x][y]; }
    int getVertical() { return vertical; }
    std::vector<Stone *> getStones();
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> getMoves();
    int getid(int x, int y)
    {
        if (c[x][y] != NULL)
            return c[x][y]->get();
        return 0;
    }
    void setShow(bool is_show_) { is_show = is_show_; }


    void show();
    void play();
    bool move(int startx, int starty, int aimx, int aimy);
    void dummy_move();
    void random_move();
    int boardEval(int);
    std::string boardHash();
    ~chessboard(); //析构函数

public:
    static const char chessword[15][4];
    static const int stonevalue[15];
    int color;         //current player color, red: -1, black: 1
    int vertical;      // first hand vertical position, -1: down, 1: up
    bool game_running; //判断结束
    bool is_show;      //computer self played chessboard will not be shown


private:
    Stone *c[10][9];//用指针调用各个棋子，实现多态
    std::stack<std::pair<std::pair<int, int>, std::pair<int, int>>> records;
    std::map<int, Stone *> capturedStones;
    std::unordered_set<std::string> seen;

private:
    //utility methods
    
    void placeStone(int color_, int vertical_);

signals:
    void update();


public slots:
    void undo();
    void flip();
};

#endif // GAMEENGINE_H
