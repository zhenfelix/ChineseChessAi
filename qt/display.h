#ifndef DISPLAY_H
#define DISPLAY_H

#include "stone.h"
#include "gameEngine.h"
#include "player.h"
#include "minimax.h"
#include <QLabel>

class Display : public QWidget
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    

public:
    QPushButton *undo_button;
    QPushButton *flip_button;
    chessboard cb;
    Player *player_a, *player_b;
    std::map<int,Player*> color2player;


    
public slots:
    void relay_receive();
    // void update();

signals:
    // void QPushButton::clicked();
    void relay_send();


private:
    void drawStones(QPainter &painter);
    bool move(int, int, int, int);
    void setup();

    QPoint center(int row, int col); // /* 返回象棋棋盘行列对应的像素坐标 */
    bool getRowCol(QPoint pt, int &row, int &col); // QPoint center(int id);

private:
    std::queue<std::pair<int, int>> message;
    int radius; /* 棋子的半径 */
    Stone *selectStone = nullptr;
};

#endif // DISPLAY_H
