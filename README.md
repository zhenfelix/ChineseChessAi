# ChineseChessAi

How to:
`qmake && make`

Features:
- 3 seperate parts, display, game engine(stones) and player;
- qt based GUI interface, responsible for listening to click event and repaint chess board, and sinal and slot in qt is responsible for communication between frontend and backend (game engine);
- signals and slot, observer pattern
- game engine and stone classes, responsible for all kinds of legal board movements (including undo and flip) and board evaluation etc;
- game engine, virtual Clone member function as a proxy to avoid virtual constructor, in implementing chessboard copy and its chess stone copy
- negmax with alpha bete pruning
- how to measure performance?
- player subclass are implemented with different AI algorithms, the bruteforce minimax can be optimized with alpha-beta pruning (~30x speed-up), increasing search depth from 4 to 5 in ~1s, furthermore, with ordered movements optimization, we can achieve another ~20x speed-up, making 6-level search in ~1s possible.
- position evaluation function, Quiescence search(heuristic search)
- with transipositon table, only ~2x improvements, not too significant. Add principal variational search, neither significant. 
- key issue solved with MCTS, using greedy rollout strategy for evaluation (significant intelligence improvements)
- MCTS tree reuse(only achieved 1/10 speedup since only 1/10 nodes in previous iteratin remains) and remove unused nodes recursively(prevent memory leak), get possible moves optimizatioin(), chessboard optimization(light and heavy version judge and move), achieved 5x speedup; chessboard copy constructor light version/shared static variables between different chessboards; get possible moves optimizatioin(stone generate valid moves); competition result of mcst-100000 and a-b-4, almost draw; position value instead of win/loss (failed try, not working); expoitation and explore balance, C_UCB parameter tuning(not working); bug to be fixed, mcts not considering cycles in simulations and could result in invalid cycling move in actual board move, and fix memory leak;
- TODO: to do parallelization;  next try alpha-beta search evaluation;  test module


ref:
- 棋盘结构 = 局面表示 + 着法移动 + 着法生成 + 特殊局面判断
- 局面评价 = 知识 + 优化的局面表示
- 搜索技术 = 完全搜索 + 静态搜索 + 启发 + 裁剪 + 选择性延伸 + 置换表 + 残局库 + 并行技术
- 其他 = 开局库 + 时间控制 + 后台思考 + 引擎协议

- [中国象棋计算机博弈关键技术分析](http://computergames.caai.cn/download/%E4%B8%AD%E5%9B%BD%E8%B1%A1%E6%A3%8B%E8%AE%A1%E7%AE%97%E6%9C%BA%E5%8D%9A%E5%BC%88%E5%85%B3%E9%94%AE%E6%8A%80%E6%9C%AF%E5%88%86%E6%9E%90.pdf)
- [Game Tree Searching and pruning: ](https://www.cs.unm.edu/~aaron/downloads/qian_search.pdf)
- [基于QT实现的alpha-beta剪枝算法搜索的象棋人机博弈游戏](https://www.write-bug.com/article/35.html)
- [MiniMax算法优化:Alpha-Beta剪枝算法](https://miketech.it/alpha-beta-pruning)
- [Best-Case Analysis of Alpha-Beta Pruning](http://www.cs.utsa.edu/~bylander/cs5233/a-b-analysis.pdf)
- [Negamax](https://en.wikipedia.org/wiki/Negamax)
- [Source code for easyAI.AI.Negamax](https://zulko.github.io/easyAI/_modules/easyAI/AI/Negamax.html)
- [Transpositional tables with alpha beta pruning](https://stackoverflow.com/questions/47052685/transpositional-tables-with-alpha-beta-pruning)
- [A REVIEW OF GAME-TREE PRUNING†](https://webdocs.cs.ualberta.ca/~tony/OldPapers/icca.Mar1986.pp3-18.pdf)
- [AI Agent for Chinese Chess](http://stanford.edu/~dengl11/resource/doc/221-Report.pdf)
- [Chinese Chess AI](https://zhiyingy.github.io/)
- [How do I choose the best algorithm for a board game like checkers?](https://ai.stackexchange.com/questions/7159/how-do-i-choose-the-best-algorithm-for-a-board-game-like-checkers)
- [Are iterative deepening, principal variation search or quiescence search extensions of alpha-beta pruning?](https://ai.stackexchange.com/questions/7231/are-iterative-deepening-principal-variation-search-or-quiescence-search-extensi?rq=1)
- [AI agents for ultimate tic-tac-toe](http://stanford.edu/~jdoan21/cs221poster.pdf)
- [Monte Carlo Tree Search Improvements](https://stackoverflow.com/questions/46006885/monte-carlo-tree-search-improvements)
- [Techniques to Parallelize Chess](http://ww2.cs.fsu.edu/~guidry/parallel_chess.pdf)
- [使用 Valgrind 检测 C++ 内存泄漏](http://senlinzhan.github.io/2017/12/31/valgrind/)
- [C/C++性能测试工具 GNU gprof](https://fooyou.github.io/document/2015/07/22/performance-tools-for-linux-cplusplus.html)









## Questions

##### what c++ tech is used in your projects?


##### what challeges did you find in your projects?


##### how did you improve performance in minimax and mcts?


##### how did you debug and solve problems in your projects?