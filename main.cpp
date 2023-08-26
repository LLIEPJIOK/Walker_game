#include <QApplication>
#include "GameInterface/game_interface.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameInterface w;
    w.show();
    return a.exec();
}
