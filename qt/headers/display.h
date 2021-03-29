#ifndef DISPLAY_H
#define DISPLAY_H

#include "stone.h"
#include "gameEngine.h"
#include "player.h"
#include "minimax.h"
#include "mcts.h"
#include <QLabel>

class Display : public QWidget
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = 0);
    void setup();

    void paintEvent(QPaintEvent*);//repaint once board changed
    void mouseReleaseEvent(QMouseEvent*);//main event loop
    void drawStones(QPainter &painter);

public:
    QPushButton *undo_button;
    QPushButton *flip_button;

    chessboard cb;
    Player *player_a, *player_b;
    std::map<int,Player*> color2player;

private:
    //utility methods within the class
    QPoint center(int row, int col); // /* 返回象棋棋盘行列对应的像素坐标 */
    bool getRowCol(QPoint pt, int &row, int &col); // QPoint center(int id);

private:
    std::queue<std::pair<int, int>> message;//queued click messages
    int radius; /* 棋子的半径 */
    Stone *selectStone = nullptr;

public slots:
    // void update();

signals:
    // void QPushButton::clicked();
};

#endif // DISPLAY_H
