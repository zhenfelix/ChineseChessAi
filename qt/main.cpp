// #include "space.h"
#include "display.h"
// #include "Game.hpp"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication app(argc, argv);
    app.setApplicationDisplayName("Chara Chess");
    Display D;
    D.show();
    
    return app.exec();
}
