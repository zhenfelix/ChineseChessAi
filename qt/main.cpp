#include "display.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication app(argc, argv);
    app.setApplicationDisplayName("Chinese Chess AI");
    Display D;
    D.show();
    
    return app.exec();
}
