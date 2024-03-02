#ifndef NEWGAME_H
#define NEWGAME_H

#include "playerssettingswindow.h"

#include <QMainWindow>
#include <QSlider>
#include <QGridLayout>
#include <vector>

class NewGame : public QMainWindow
{
    Q_OBJECT
public:
    NewGame(QWidget *parent = nullptr);
    ~NewGame();
    void update_lang();
private:
    QLabel *label_choose;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QHBoxLayout *label_hblay;
    QSlider *slider;
    QPushButton *btn_next;
    QPushButton *btn_prev;
    QVBoxLayout *vblay;
    QVBoxLayout *all_vblay;
    QHBoxLayout *hblay;
    QHBoxLayout *btn_hblay;
    QGridLayout *gridlay;
    QWidget *widget;
    PlayersSettingsWindow *player_set_window;
signals:
    void open_menu_signal();
    void start_lobby(int players);
public slots:
    void go_choose_players();
    void show_new_game();
};

#endif // NEWGAME_H
