//https://blog.csdn.net/Originum/article/details/80356452
//C++中国象棋


#include "stone.h"
#include "gameEngine.h"

using namespace std;

char chessboard::chessword[15][4] = {"帥", "相", "炮", "仕", "俥", "傌", "兵", "", "卒", "馬", "車", "士", "砲", "象", "将"}; //名字
int chessboard::stonevalue[15] = {-10000,-4,-9,-4,-18,-8,-2,0,2,8,18,4,9,4,10000};//stone value
// bool chessboard::end = true;
// int chessboard::color = -1;    //initialize first hand color
// int chessboard::vertical = -1; //initialize first hand vertical position

// chessboard::chessboard() { memset(c, NULL, sizeof(c)); srand( (unsigned)time(NULL) ); }; //把指针初始化为零指针
// // chessboard::chessboard() { memset(c, 0, sizeof(c)); }; //把指针初始化为零指针

chessboard::chessboard(bool is_show_=true)
    : is_show(is_show_)
{
    memset(c, NULL, sizeof(c));
    srand((unsigned)time(NULL));
    color = -1;
    game_running = true;
    vertical = -1;
}; //把指针初始化为零指针
// chessboard::chessboard() { memset(c, 0, sizeof(c)); }; //把指针初始化为零指针


bool chessboard::move(int startx, int starty, int aimx, int aimy)
{
    if (startx >= 0 && startx < 10 && starty >= 0 && starty < 9 //初步判断传入的点是否符合规则
        && aimx >= 0 && aimx < 10 && aimy >= 0 && aimy < 9 && getid(startx, starty) && getid(startx, starty) * color > 0 && c[startx][starty]->judge_move(*this, startx, starty, aimx, aimy))
    {
        if (c[aimx][aimy] != NULL)
            {
                // delete c[aimx][aimy]; //吃子
                capturedStones.insert(std::pair<int, Stone *>(records.size(), c[aimx][aimy]));
            }
        c[aimx][aimy] = c[startx][starty];
        c[aimx][aimy]->setPos(aimx,aimy);
        c[startx][starty] = NULL;
        std::pair<int, int> start_xy = {startx, starty};
        std::pair<int, int> aim_xy = {aimx, aimy};
        records.push({start_xy, aim_xy});
        color *= -1;
        vertical *= -1;
        show();
        return true;
    }
    // cout << "走法错误，不符合规则" << endl;
    return false;
}

void chessboard::undo()
{
    std::cout << "hello, undo detected!" << std::endl;
    if (records.empty())
    {
        std::cout << "already the first step!" << std::endl;
        return;
    }
    color *= -1;
    vertical *= -1;
    // int aimx, aimy, startx, starty;
    // std::pair<int, int> start_xy, aim_xy;
    // start_xy = records.top().first, aim_xy = records.top().second;
    auto [start_xy, aim_xy] = records.top();
    records.pop();
    auto [startx, starty] = start_xy;
    auto [aimx, aimy] = aim_xy;
    // std::cout << startx << ", " << starty << ", " << aimx << ", " << aimy << std::endl;
    c[startx][starty] = c[aimx][aimy];
    c[startx][starty]->setPos(startx, starty);
    c[aimx][aimy] = nullptr;
    if (capturedStones.find(records.size()) != capturedStones.end())
    {
        c[aimx][aimy] = capturedStones[records.size()];
        capturedStones.erase(records.size());
    }
    show();
}



void chessboard::dummy_move()
{
    
    
    std::cout << "dummy move in chessboard!" << std::endl;
    color *= -1;
    vertical *= -1;
}

void chessboard::random_move()
{
    int row_start, col_start, row_aim, col_aim;
    do
    {
        row_start = rand()%10;
        col_start = rand()%9;
        row_aim = rand() % 10;
        col_aim = rand() % 9;
    } while (!move(row_start,col_start,row_aim,col_aim));
    
    std::cout << "random move in chessboard!" << std::endl;
    
}


void chessboard::flip()
{
    std::cout << "flip detected!" << std::endl;
    Stone *c_excahnge[10][9];
    
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
            c_excahnge[9 - i][8-j] = c[i][j];
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
            {
                c[i][j] = nullptr;
                c[i][j] = c_excahnge[i][j];
                if(c[i][j])c[i][j]->setPos(i,j);
            }
    vertical *= -1;
    show();
    
}

int chessboard::boardEval(int maximizer_color) //color of maximizer player
{
    int score = 0;
    std::vector<Stone *> st = getStones();
    for (auto s : st)
    {
        if (!s)
            continue;
        int id = s->get();
        score += maximizer_color * chessboard::stonevalue[id + 7];
    }

    return score;
}

chessboard ::~chessboard()
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
            if (c[i][j] != NULL)
            {
                delete c[i][j];
                c[i][j] = NULL;
            }
}

void chessboard::placeStone(int color_, int vertical_)
{
    if(vertical_ == -1)
    {
        c[6][8] = new soldier(color_+1,6,8);
        c[6][0] = new soldier(color_+1,6,0);
        c[6][2] = new soldier(color_+1,6,2);
        c[6][4] = new soldier(color_+1,6,4);
        c[6][6] = new soldier(color_+1,6,6);
        c[7][1] = new cannon(color_+1,7,1);
        c[7][7] = new cannon(color_+1,7,7);
        c[9][4] = new general(color_+1,9,4);
        c[9][3] = new guard(color_+1,9,3);
        c[9][5] = new guard(color_+1,9,5);
        c[9][2] = new elephant(color_+1,9,2);
        c[9][6] = new elephant(color_+1,9,6);
        c[9][1] = new horse(color_+1,9,1);
        c[9][7] = new horse(color_+1,9,7);
        c[9][0] = new rook(color_+1,9,0);
        c[9][8] = new rook(color_+1,9,8);
    }
    else
    {
        c[3][8] = new soldier(color_ + 1, 3,8);
        c[3][0] = new soldier(color_ + 1, 3,0);
        c[3][2] = new soldier(color_ + 1, 3,2);
        c[3][4] = new soldier(color_ + 1, 3,4);
        c[3][6] = new soldier(color_ + 1, 3,6);
        c[2][1] = new cannon(color_ + 1, 2,1);
        c[2][7] = new cannon(color_ + 1, 2,7);
        c[0][4] = new general(color_ + 1, 0,4);
        c[0][3] = new guard(color_ + 1, 0, 3);
        c[0][5] = new guard(color_ + 1, 0, 5);
        c[0][2] = new elephant(color_ + 1, 0, 2);
        c[0][6] = new elephant(color_ + 1, 0, 6);
        c[0][1] = new horse(color_ + 1, 0, 1);
        c[0][7] = new horse(color_ + 1, 0, 7);
        c[0][0] = new rook(color_ + 1, 0, 0);
        c[0][8] = new rook(color_ + 1, 0, 8);
    }
    
}

void chessboard ::init()
{ //初始化，棋子的生成
    placeStone(color,vertical);
    placeStone(-color,-vertical);
}

std::vector<Stone*> chessboard::getStones()
{
    std::vector<Stone *> res;
    for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (c[row][col]) res.push_back(c[row][col]);
        }
        
    }
    return res;
}

void chessboard ::show()
{
    if (!is_show)
        return;
    cout << "吴  零一二三四五六七八" << endl
         << endl;
    char num[10][4] = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九"};
    for (int i = 0; i < 10; i++)
    {
        if (i == 5)
            cout << "    ——楚 河 汉 界———" << endl;
        cout << num[i] << "  ";
        for (int j = 0; j < 9; j++)
        {
            if (c[i][j] != NULL)
                cout << chessboard::chessword[c[i][j]->get() + 7];
            else if ((i == 1 && j == 4) || (i == 8 && j == 4))
                cout << "米";
            else
                cout << "十";
        }
        cout << endl;
    }
    std::cout << boardEval(-1) << std::endl;
    emit update();//update qt gui signal
}

void chessboard::play()
{
    this->init();
    this->show();
    do
    {
        int startx, starty, aimx, aimy;
        int sid, aid;
        do
        {
            sid = aid = 0;
            cout << "请输入起始棋子位置与目标位置的坐标：" << endl;
            cin >> startx >> starty >> aimx >> aimy;
        } while (!this->move(startx, starty, aimx, aimy));
        this->show();
    } while (game_running);
    cout << "结束，赢家是Player" << (color == 1 ? 1 : 2) << endl;
}

// int main()
// {
//     chessboard C;
//     C.play();
// }
