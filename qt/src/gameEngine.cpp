//https://blog.csdn.net/Originum/article/details/80356452
//C++中国象棋


#include "stone.h"
#include "gameEngine.h"
#include <fstream>
#include <string>

using namespace std;

// const char chessboard::chessword[15][4] = {"帥", "相", "炮", "仕", "俥", "傌", "兵", "", "卒", "馬", "車", "士", "砲", "象", "将"}; //名字
// const int chessboard::stonevalue[15] = {-10000,-4,-9,-4,-18,-8,-2,0,2,8,18,4,9,4,10000};//stone value

const char chessboard::chessword[15][4] = {"兵", "相", "仕", "炮", "傌", "俥", "帥", "", "将", "車", "馬", "砲", "士", "象", "卒"}; //名字
const int chessboard::stonevalue[15] = {-2, -4, -4, -8, -9, -18, -10000, 0, 10000, 18, 9, 8, 4, 4, 2};                              //stone value

chessboard::chessboard(bool is_show_=true)
    : is_show(is_show_)
{
    memset(c, NULL, sizeof(c));
    // srand((unsigned)time(NULL));
    srand((unsigned) 2021);
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
                if (std::abs(c[aimx][aimy]->get()) == 1)
                {
                    game_running = false;//game over once general captured
                }
            }
        c[aimx][aimy] = c[startx][starty];
        c[aimx][aimy]->setPos(aimx,aimy);
        c[startx][starty] = NULL;
        pos_type start_xy = {startx, starty};
        pos_type aim_xy = {aimx, aimy};
        records.push({start_xy, aim_xy});
        color *= -1;
        vertical *= -1;
        if (checkCycle(startx, starty, aimx, aimy))
        {
            undo();
            return false;
        }
        show();
        return true;
    }
    // cout << "走法错误，不符合规则" << endl;
    return false;
}

bool chessboard::checkCycle(int startx, int starty, int aimx, int aimy)
{
    std::string board_hash = boardHash();
    seen.insert(board_hash); //to avoid cyclic moves
    if (abs(getid(startx, starty)) == 1)return false;
    if (seen.count(board_hash) > 1)
        return true;
    return false;
}

void chessboard::undo()
{
//    std::cout << "hello, undo detected!" << std::endl;
    if (records.empty())
    {
        std::cout << "already the first step!" << std::endl;
        return;
    }

    std::string board_hash = boardHash();
    seen.erase(seen.find(board_hash)); //buggy!!! multiset erase usages!!! erase only one element

    color *= -1;
    vertical *= -1;
    auto &[start_xy, aim_xy] = records.top();
    records.pop();
    auto &[startx, starty] = start_xy;
    auto &[aimx, aimy] = aim_xy;
    // std::cout << startx << ", " << starty << ", " << aimx << ", " << aimy << std::endl;
    c[startx][starty] = c[aimx][aimy];
    c[startx][starty]->setPos(startx, starty);
    c[aimx][aimy] = nullptr;
    if (capturedStones.find(records.size()) != capturedStones.end())
    {
        c[aimx][aimy] = capturedStones[records.size()];
        capturedStones.erase(records.size());
        if (std::abs(c[aimx][aimy]->get()) == 1)
        {
            game_running = true; //restore game
        }
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

int chessboard::boardEvalNegMax() //negmax board eval
{
    int score = 0;
    std::vector<Stone *> st = getStones();
    for (auto s : st)
    {
        if (!s)
            continue;
        int id = s->get();
        score += color * chessboard::stonevalue[id + 7];
    }

    return score;
}

int chessboard::boardPosEval()
{
    int score = 0;
    std::vector<Stone *> st = getStones();
    for (auto s : st)
    {
        if (!s)
            continue;
        int id = s->get();
        int vertical_ = s->getVer();
        int row, col;
        s->getPos(row,col);
        if (id * color > 0)
            score += stone2val[abs(id) * vertical_][row][col];
        else
            score -= stone2val[abs(id) * vertical_][row][col];
    }

    return score;
}

std::string chessboard::boardHash()
{
    std::string res = color == -1 ? "r" : "b";

    std::vector<Stone *> st = getStones();
    for (auto s : st)
    {
        if (!s)
            continue;
        int id = s->get();
        int row, col;
        char ch;
        s->getPos(row,col);
        res += chessboard::chessword[id + 7];
        ch = '0' + row;
        res += ch;
        ch = '0' + col;
        res += ch;
    }
    return res;
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

void chessboard::flipMat(std::vector<std::vector<int>> &mat)
{
    int n = mat.size();
    int m = mat[0].size();
    for (int i = 0; i < n/2; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int tmp = mat[i][j];
            mat[i][j] = mat[n-1-i][m-1-j];
            mat[n-1-i][m-1-j] = tmp;
        }
        
    }
    
}

void chessboard::readPosVal()
{
    std::ifstream infile("pos_value.txt");
    int tmp;
    for (int idx = 1; idx <= 7; idx++)
    {
        std::vector<std::vector<int>> val_mat;
        for (int row = 0; row < 10; row++)
        {
            std::vector<int> row_val;
            for (int col = 0; col < 9; col++)
            {
                infile >> tmp;
                row_val.push_back(tmp);
            }
            val_mat.push_back(row_val);
        }
        stone2val.insert({idx,val_mat});
        // stone2val[idx] = val_mat;
        flipMat(val_mat);
        stone2val.insert({-idx,val_mat});
        // stone2val[-idx] = val_mat;
    }
    
}

void chessboard::readBoard()
{
    std::ifstream infile("init.txt");
    int tmp, vertical_, color_;
    for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            infile >> tmp;
            if(tmp == 0)continue;
            if(tmp < 0)color_ = -1;
            else color_ = 1;
            if(row < 5)vertical_ = 1;
            else vertical_ = -1;
            if(abs(tmp) == 1)
                c[row][col] = new general(color_ + 1, row, col, vertical_);
            else if(abs(tmp) == 2)
                c[row][col] = new rook(color_ + 1, row, col, vertical_);
            else if(abs(tmp) == 3)
                c[row][col] = new horse(color_ + 1, row, col, vertical_);
            else if(abs(tmp) == 4)
                c[row][col] = new cannon(color_ + 1, row, col, vertical_);
            else if(abs(tmp) == 5)
                c[row][col] = new guard(color_ + 1, row, col, vertical_);
            else if(abs(tmp) == 6)
                c[row][col] = new elephant(color_ + 1, row, col, vertical_);
            else
                c[row][col] = new soldier(color_ + 1, row, col, vertical_);
        }
        
    }
    
}


void chessboard ::init()
{ //初始化，棋子的生成
    readBoard();
    readPosVal();
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
    // std::sort(res.begin(), res.end(), [](Stone *a, Stone *b) { return abs(chessboard::stonevalue[a->get() + 7]) > abs(chessboard::stonevalue[b->get() + 7]); });
    //priotize stones with larger value, however not improving performance
    // for(auto s: res)std::cout << chessboard::chessword[s->get()+7] << " ";
    // std::cout << std::endl;
    return res;
}

std::vector<std::pair<pos_type, pos_type>> chessboard::getMoves()
{
    std::vector<std::pair<pos_type, pos_type>> candidates;
    std::vector<Stone*> candidate_stones = getStones();
    for(auto s: candidate_stones)
    {
        int start_row, start_col;
        s->getPos(start_row,start_col);
        for (int aim_row = 0; aim_row < 10; aim_row++)
        {
            for (int aim_col = 0; aim_col < 9; aim_col++)
            {
                if(move(start_row,start_col,aim_row,aim_col))
                {
                    candidates.push_back({{start_row,start_col},{aim_row,aim_col}});
                    undo();
                }
            }
            
        }
        
    }
    

    return candidates;
}

void chessboard::sortMoves(std::vector<std::pair<pos_type, pos_type>> &candidates)
{
    std::sort(candidates.begin(), candidates.end(), [&](std::pair<pos_type, pos_type> &a, std::pair<pos_type, pos_type> &b) {
        auto &[a_start_xy, a_aim_xy] = a;
        auto &[b_start_xy, b_aim_xy] = b;
        auto &[a_x, a_y] = a_aim_xy;
        auto &[b_x, b_y] = b_aim_xy;
        int va = 0, vb = 0;
        if (c[a_x][a_y])
            va = abs(chessboard::stonevalue[c[a_x][a_y]->get() + 7]);
        if (c[b_x][b_y])
            vb = abs(chessboard::stonevalue[c[b_x][b_y]->get() + 7]);
        return va > vb;
    }); //reordering movements to improve alpha beta performance up to ~30x

    // for(auto &[start_xy, aim_xy]: candidates)
    // {
    //     auto &[x,y] = aim_xy;
    //     std::cout << chessboard::chessword[c[x][y]->get()+7] << " ";
    // }
    // std::cout << std::endl;

    return;
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
    std::cout << "current board score from current player perspective is " << boardPosEval() << std::endl;
    // std::cout << "current board score (maximizer color" << MAXIMIZER_COLOR << ") is " << boardEval(MAXIMIZER_COLOR) << std::endl; //default maximizer color is -1
    // std::cout << boardHash() << std::endl;
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
