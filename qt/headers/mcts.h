#ifndef MCTS_H
#define MCTS_H

#include "player.h"
#include <QObject>

#define MAXSTEP 0


struct MctsNode
{
    int n;
    int wins;
    int score;
    int color;
    MctsNode *parent;
    move_type pmove;
    std::vector<MctsNode*> children;
    std::vector<move_type> possible_moves;

    MctsNode(int color_, MctsNode *parent_, move_type pmove_, std::vector<move_type> possible_moves_)
        : n(0), wins(0), score(0), color(color_), parent(parent_), pmove(pmove_), possible_moves(possible_moves_)
    {
//        int sz = possible_moves.size();//random permutation of possible moves
//        for (int i = sz; i > 0; i--)
//        {
//            int idx = rand()%i;
//            move_type tmp = possible_moves[i];
//            possible_moves[i] = possible_moves[idx];
//            possible_moves[idx] = tmp;
//        }
        
    }
    void show_children()
    {
        std::sort(children.begin(), children.end(), [&](MctsNode* a, MctsNode* b){
            return a->n > b->n;
        });
        for (auto it = children.begin(); it != children.end(); it++)
        {
            std::cout << (*it)->pmove << " count: " << (*it)->n << " wins: " << (*it)->wins << " score: " << (*it)->score << std::endl;
        }
    }

    ~MctsNode(){
        for(auto it = children.begin(); it != children.end(); it++){
            delete (*it);
        }
    }
};


class Mcts : public Player
{
    Q_OBJECT

public:
    Mcts(bool isHuman_, chessboard &cb_, int sim_cnt_=10000); //set simlulation count
    void think();
    


public:
    

private:
    int sim_cnt; //simlulation count
    MctsNode *root;
    chessboard *cb_ptr;

private:
    move_type calcMcts();
    move_type calcMcts_score();
    MctsNode* moveRoot();
    int count(MctsNode*);
    MctsNode *select();
    MctsNode *select_score();
    MctsNode *ucb(MctsNode*,double);
    MctsNode *ucb_score(MctsNode *, double);
    MctsNode *chooseChild();
    int rollout();
    int rollout_score();
    void backpropagate(MctsNode *, int);
    void backpropagate_score(MctsNode *, int);

    // public slots:

    // signals:
    //     void send();
};

#endif // MCTS_H
