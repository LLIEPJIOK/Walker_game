#ifndef MENU_H
#define MENU_H

#include "exitwindow.h"
#include "newgame.h"
#include "credits.h"
#include "load.h"

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    Menu(QWidget *parent = nullptr);
    ~Menu();

private:
//    QPushButton *btn_new_game;
//    QPushButton *btn_load;
    QPushButton *btn_lobby;
    QPushButton *btn_connect;

    QPushButton *btn_titers;
    QPushButton *btn_exit;
    QPushButton *btn_lang;
    QVBoxLayout *vblay;
    ExitWindow *exit_window;
    NewGame *new_game;
    QWidget *widget;
    Credits *titers;
    Load* load;
    static QTranslator translator;

    bool eng;

public slots:
    void open_exit_window();
    void open_titers();
    void open_new_game();
    void open_load();
    void menu_enable();
    void change_lang();
    void connectTo();
    void create_lobby(int qnt);
protected:
    void paintEvent(QPaintEvent* event);
    void update_lang();

signals:
    void hide_main_window();
};

#endif // MENU_H
