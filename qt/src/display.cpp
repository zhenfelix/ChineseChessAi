#include "display.h"

// #define TIME_SELF

Display::Display(QWidget *parent) 
    : QWidget(parent), cb(true)
{
    setup();
    cb.init();
}

void Display::setup()
{
    player_a = new Player(true, cb);//configure players
    // player_a = new Player(false, cb); //random move player to debug, test reproducibility
    // player_a = new Minimax(false, cb, 5); //weaker minimax player to debug
    // player_b = new Player(true,cb);
    // player_b = new Minimax(false, cb, 5);
    // player_b = new Mcts(false, cb, 10000);
    player_b = new Mcts(false, cb);

    color2player[-1] = player_a;
    color2player[1] = player_b;

    radius = 30;
    this->resize(25 * radius, 22 * radius);
    this->setStyleSheet("background-color: rgb(235, 187, 83)");

    // QWidget *wdg = new QWidget(this);
    undo_button = new QPushButton(this);
    undo_button->move(21 * radius, 10*radius);
    undo_button->resize(2*radius, 1*radius);
    undo_button->setText(tr("Undo"));
    connect(undo_button, SIGNAL(clicked()), &cb, SLOT(undo()));

    flip_button = new QPushButton(this);
    flip_button->move(21 * radius, 12 * radius);
    flip_button->resize(2 * radius, 1 * radius);
    flip_button->setText(tr("Flip"));
    connect(flip_button, SIGNAL(clicked()),&cb, SLOT(flip()));
    connect(&cb, SIGNAL(update()), this, SLOT(repaint()));
}

void Display::paintEvent(QPaintEvent *)
{
    int d = 2 * radius;
    
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 2));
    //画10横线
    for (int i = 1; i <= 10; i++)
    {
        painter.drawLine(QPoint(d, i * d), QPoint(9 * d, i * d));
    }

    for (int i = 1; i <= 9; i++)
    {
        if (i == 1 || i == 9)
            painter.drawLine(QPoint(i * d, d), QPoint(i * d, 10 * d));
        else
        {
            painter.drawLine(QPoint(i * d, d), QPoint(i * d, 5 * d));
            painter.drawLine(QPoint(i * d, 6 * d), QPoint(i * d, 10 * d));
        }
    }

    //九宫格
    painter.drawLine(QPoint(4 * d, 1 * d), QPoint(6 * d, 3 * d));
    painter.drawLine(QPoint(6 * d, 1 * d), QPoint(4 * d, 3 * d));

    painter.drawLine(QPoint(4 * d, 8 * d), QPoint(6 * d, 10 * d));
    painter.drawLine(QPoint(6 * d, 8 * d), QPoint(4 * d, 10 * d));

    QRect label1 = QRect(3 * radius, 10. * radius, radius * 4, radius * 1.6);
    painter.setFont(QFont("Helvetica", 0.8 * radius, 20));
    painter.drawText(label1, "楚河", QTextOption(Qt::AlignCenter));

    QRect label2 = QRect(13 * radius, 10. * radius, radius * 4, radius * 1.6);
    painter.setFont(QFont("Helvetica", 0.8 * radius, 20));
    painter.drawText(label2, "汉界", QTextOption(Qt::AlignCenter));

    drawStones(painter);

}


void Display::mouseReleaseEvent(QMouseEvent *qMouse)
{
    #ifdef TIME_SELF
    std::clock_t start;
    double duration;
    start = std::clock();//test two computer player self-play time duration
    #endif


    if(!cb.game_running)
    {
        std::cout << "Game Over!" << std::endl;
        return;
    }
    QPoint pt = qMouse->pos(); //鼠标点位置
    int row, col;
    
    // std::cout << pt.x() << ", " << pt.y() << std::endl;
    if(getRowCol(pt,row,col)){
        selectStone = cb.get(row,col);
        // std::cout << row << ", " << col << std::endl;

        message.push({row, col});
        if(message.size() == 2)
        {
            auto from = message.front();
            auto to = message.back();
            // std::cout << from.first << ", " << from.second << ", " << to.first << ", " << to.second << std::endl;
            if (cb.move(from.first, from.second, to.first, to.second))
            {
                // cb.show();
                message.pop();
                if(selectStone)selectStone = nullptr;
            } 
            message.pop();
        }
    }
    update();//highlight selected color
    while (!color2player[cb.color]->isHuman && cb.game_running)
    {
        Player *player_x = color2player[cb.color];
        player_x->think();
        // cb.show();
        // repaint();
        // usleep(1000000); 
        //The difference between QT update and repaint
        // duration = (std::clock() - start) / (double)CLOCKS_PER_SEC; //test two computer player self-play time duration
        // std::cout << "\n\n\nself-play took " << duration << " seconds for now\n\n\n";

    } // to support human vs computer and computer vs computer

    #ifdef TIME_SELF
    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC; //test two computer player self-play time duration
    std::cout << "\n\n\nself-play took " << duration << " seconds in total\n\n\n";
    #endif
}

void Display::drawStones(QPainter &painter)
{
    int row, col;
    std::vector<Stone*> st = cb.getStones();
    for(auto s: st){
        s->getPos(row, col);
        int id = s->get();
        QString name = QString::fromUtf8(chessboard::chessword[id+7]);
        // std::cout << chessboard::chessword[id + 7] << id << std::endl;
        int x = 2*radius*(col+1);
        int y = 2*radius*(row+1);
        // painter.setPen(Qt::black);
        if (s == selectStone)
            painter.setBrush(QColor(200, 154, 0));
        else
            painter.setBrush(QColor(249, 217, 170));

        if (id < 0)
            painter.setPen(QPen(Qt::red, 2));
        else
            painter.setPen(QPen(Qt::black, 2));

        int delta = 3;
        painter.drawEllipse(x - radius, y - radius , (radius) * 2, (radius) * 2);
        painter.drawEllipse(x - radius + delta, y - radius + delta, (radius - delta) * 2, (radius-delta)*2);
        QRect rect = QRect(x - 2 * radius, y - 2 * radius, radius * 4, radius * 3.7);
        painter.setFont(QFont("Helvetica", radius, 20));
        painter.drawText(rect, name, QTextOption(Qt::AlignCenter));
    }
}

QPoint Display::center(int row, int col)
{
    QPoint ret;
    ret.rx() = (col + 1) * radius * 2;
    ret.ry() = (row + 1) * radius * 2;
    return ret;
}


bool Display::getRowCol(QPoint pt, int &row, int &col)
{
    for (row = 0; row <= 9; row++)
    {
        for (col = 0; col <= 8; col++)
        {
            QPoint c = center(row, col);
            int dx = c.x() - pt.x();
            int dy = c.y() - pt.y();
            int dist = dx * dx + dy * dy;
            if (dist <radius * radius)
                return true;
        }
    }
    return false;
}
