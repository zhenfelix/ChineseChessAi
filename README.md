# ChineseChessAi

How to:
`qmake && make`

Features:
- 3 seperate parts, display, game engine(stones) and player;
- qt based GUI interface, responsible for listening to click event and repaint chess board, and sinal and slot in qt is responsible for communication between frontend and backend (game engine);
- game engine and stone classes, responsible for all kinds of legal board movements (including undo and flip) and board evaluation etc;
- player subclass are implemented with different AI algorithms, the bruteforce minimax can be optimized with alpha-beta pruning (~30x speed-up), increasing search depth from 4 to 5 in ~1s, furthermore, with ordered movements optimization, we can achieve another ~20x speed-up, making 6-level search in ~1s possible.

ref:
棋盘结构 = 局面表示 + 着法移动 + 着法生成 + 特殊局面判断
局面评价 = 知识 + 优化的局面表示
搜索技术 = 完全搜索 + 静态搜索 + 启发 + 裁剪 + 选择性延伸 + 置换表 + 残局库 + 并行技术
其他 = 开局库 + 时间控制 + 后台思考 + 引擎协议

- [中国象棋计算机博弈关键技术分析](http://computergames.caai.cn/download/%E4%B8%AD%E5%9B%BD%E8%B1%A1%E6%A3%8B%E8%AE%A1%E7%AE%97%E6%9C%BA%E5%8D%9A%E5%BC%88%E5%85%B3%E9%94%AE%E6%8A%80%E6%9C%AF%E5%88%86%E6%9E%90.pdf)
- [基于QT实现的alpha-beta剪枝算法搜索的象棋人机博弈游戏](https://www.write-bug.com/article/35.html)
- [MiniMax算法优化:Alpha-Beta剪枝算法](https://miketech.it/alpha-beta-pruning)
- [Best-Case Analysis of Alpha-Beta Pruning](http://www.cs.utsa.edu/~bylander/cs5233/a-b-analysis.pdf)
- [Negamax](https://en.wikipedia.org/wiki/Negamax)
- [Transpositional tables with alpha beta pruning](https://stackoverflow.com/questions/47052685/transpositional-tables-with-alpha-beta-pruning)
- [A REVIEW OF GAME-TREE PRUNING†](https://webdocs.cs.ualberta.ca/~tony/OldPapers/icca.Mar1986.pp3-18.pdf)
- [AI Agent for Chinese Chess](http://stanford.edu/~dengl11/resource/doc/221-Report.pdf)
- [Chinese Chess AI](https://zhiyingy.github.io/)