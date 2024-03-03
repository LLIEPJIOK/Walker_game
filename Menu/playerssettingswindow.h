#ifndef PLAYERSSETTINGSWINDOW_H
#define PLAYERSSETTINGSWINDOW_H

#include "Engine/Transceiver.h"
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
    void update_lang();
    std::vector<game_msg> gather_info();
private:
    int players_number;
    QHBoxLayout *buttons;
    QHBoxLayout *settings;
    QVBoxLayout *vl;
    QPushButton *back;
    QPushButton *start_the_game;
    std::vector<std::pair<std::string, std::string>> data;
    QVector <bool> players_ready;
    QVector <InitialSettings*> in_set;
    void clear();
signals:
    void go_back();
    void all_are_ready(std::vector<std::pair<std::string, std::string>>);
    void sync_data(std::vector<game_msg> states, int id);
public slots:
    void set_player(int id, bool is_ready, std::string name, std::string stats);
    void start_button_is_clicked();
    void check_all_ready();
    void set_connected(int id, bool val);
    void someone_connected(int id);
    void someone_disconnected(int id);
    void update_access();
    void get_info_msg(game_msg msg);
    void sync(int id);

};

#endif // PLAYERSSETTINGSWINDOW_H
