#ifndef PLAYERSSETTINGSWINDOW_H
#define PLAYERSSETTINGSWINDOW_H

#include "initialsettings.h"

#include <QList>
#include <QApplication>
#include <QScreen>
#include <QPalette>

class PlayersSettingsWindow : public QWidget
{
    Q_OBJECT
public:
    PlayersSettingsWindow(int _players_number, QWidget *parent = nullptr);
    ~PlayersSettingsWindow();
    void set_players(int number);
    void add_player(int number);
private:
    int players_number;
    QHBoxLayout *buttons;
    QHBoxLayout *settings;
    QVBoxLayout *vl;
    QPushButton *back;
    QPushButton *start_the_game;
    std::vector<std::pair<std::string, std::string>> data;
    QList <bool> players_ready;
    QList <InitialSettings*> in_set;
    void clear();
signals:
    void go_back();
    void all_are_ready(std::vector<std::pair<std::string, std::string>>);
private slots:
    void set_player(int id, bool is_ready, std::string name, std::string stats);
    void start_button_is_clicked();
    void check_all_ready();
};

#endif // PLAYERSSETTINGSWINDOW_H
