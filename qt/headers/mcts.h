#ifndef MCTS_H
#define MCTS_H

#include "player.h"
#include <QObject>

#define MAXSTEP 400


struct MctsNode
{
    int n;
    int wins;
    int color;
    MctsNode *parent;
    std::pair<pos_type,pos_type> pmove;
    std::vector<MctsNode*> children;
    std::vector<std::pair<pos_type,pos_type>> possible_moves;

    MctsNode(int color_, MctsNode *parent_, std::pair<pos_type,pos_type> pmove_, std::vector<std::pair<pos_type,pos_type>> possible_moves_)
        : n(0), wins(0), color(color_), parent(parent_), pmove(pmove_), possible_moves(possible_moves_)
    {
//        int sz = possible_moves.size();//random permutation of possible moves
//        for (int i = sz; i > 0; i--)
//        {
//            int idx = rand()%i;
//            std::pair<pos_type,pos_type> tmp = possible_moves[i];
//            possible_moves[i] = possible_moves[idx];
//            possible_moves[idx] = tmp;
//        }
        
    }
};


class Mcts : public Player
{
    Q_OBJECT

public:
    Mcts(bool isHuman_, chessboard &cb_, int sim_cnt_);//set simlulation count
    void think();
    


public:
    

private:
    int sim_cnt; //simlulation count
    MctsNode *root;
    chessboard cb_my;

private:
    std::pair<pos_type,pos_type> calcMcts();
    MctsNode *select();
    MctsNode *ucb(MctsNode*,double);
    MctsNode *chooseChild(MctsNode *);
    int rollout();
    void backpropagate(MctsNode *, int);

    // public slots:

    // signals:
    //     void send();
};

#endif // MCTS_H
