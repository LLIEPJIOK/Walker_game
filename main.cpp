#include <QApplication>
//#include "Events/event_window.h"
#include "GameInterface/game_interface.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Player * pl = new Player("Никита");
    //Event_window * w = new Event_window(nullptr, pl, Events::get_Events()->get_events()->at("experiment"));
    GameInterface w;
    w.show();
    return a.exec();
}
