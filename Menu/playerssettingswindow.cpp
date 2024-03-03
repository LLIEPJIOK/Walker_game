#include "playerssettingswindow.h"
#include "general.h"
#include "Engine/Transceiver.h"

PlayersSettingsWindow::PlayersSettingsWindow(int _players_number, QWidget *parent)
    : QWidget(parent)
{
    QFont btn_font ("Arial", 14, QFont::Normal, 1);
    setStyleSheet("QPushButton:active   {color: white;}"
                  "QPushButton:disabled {color: gray;}"
                  "QPushButton:hover    {color: rgb(255, 178, 102);}"
                  "QLabel               {color: white;}");

    players_number = _players_number;

    settings = new QHBoxLayout;

    for(int i = 0; i < players_number; i++)
    {
        InitialSettings* is = new InitialSettings(i);
        settings->addWidget(is);
        in_set.push_back(is);
        connect(is, &InitialSettings::ready, this, &PlayersSettingsWindow::set_player);
    }
    data.resize(players_number);
    players_ready.resize(players_number);

    start_the_game = new QPushButton(tr("Start!"));
    start_the_game->setFlat(1);
    start_the_game->setEnabled(0);
    start_the_game->setFont(btn_font);

    QPalette pal = start_the_game->palette();
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::gray);
    start_the_game->setPalette(pal);

    back = new QPushButton(tr("Return"));
    back->setFlat(1);
    back->setFont(btn_font);

    buttons = new QHBoxLayout;
    buttons->addWidget(back);
    buttons->addWidget(start_the_game);
    buttons->setSpacing(QApplication::screens().at(0)->size().width() - 300);

    vl = new QVBoxLayout;
    vl->addLayout(settings);
    vl->addLayout(buttons);
    vl->setContentsMargins(0, 0, 0, 30);
    setLayout(vl);

    connect(back, &QPushButton::clicked, this, &PlayersSettingsWindow::go_back);
    connect(start_the_game, &QPushButton::clicked, this, &PlayersSettingsWindow::start_button_is_clicked);
    connect(this, &PlayersSettingsWindow::all_are_ready, General::get_general(), &General::start_game);
    connect(Transceiver::get_transceiver(), &Transceiver::connect_successful, this, &PlayersSettingsWindow::someone_connected);
    connect(Transceiver::get_transceiver(), &Transceiver::user_disconnected, this, &PlayersSettingsWindow::someone_disconnected);
    connect(Transceiver::get_transceiver(), &Transceiver::ready_check, this, &PlayersSettingsWindow::get_info_msg);
    connect(Transceiver::get_transceiver(), &Transceiver::lobby_sync_init, this, &PlayersSettingsWindow::sync);
    connect(Transceiver::get_transceiver(), &Transceiver::set_connected, this, &PlayersSettingsWindow::set_connected);
    connect(this, &PlayersSettingsWindow::sync_data, Transceiver::get_transceiver(), &Transceiver::lobby_sync);

    update_access();

}

PlayersSettingsWindow::~PlayersSettingsWindow()
{
    for (int i = 0; i < players_number; ++i)
        delete in_set[i];
}

void PlayersSettingsWindow::set_players(int number)
{
    if (number > players_number)
    {
        add_player(number - players_number);
    }
    if (number < players_number)
    {
        while(number < players_number)
        {
            delete in_set.back();
            in_set.pop_back();
            players_ready.pop_back();
            data.pop_back();
            players_number--;
        }
        clear();
    }
    check_all_ready();
}

void PlayersSettingsWindow::add_player(int number)
{
    while(number--)
    {
        InitialSettings* is = new InitialSettings(players_number);
        settings->addWidget(is);
        in_set.push_back(is);
        players_ready.push_back(0);
        data.push_back({"", ""});
        connect(is, &InitialSettings::ready, this, &PlayersSettingsWindow::set_player);
        players_number++;
    }
}

void PlayersSettingsWindow::update_lang()
{
    start_the_game->setText(tr("Start!"));
    back->setText(tr("Return"));
    for (auto& i : in_set)
        i->update_lang();

    //set_players(0);
}

std::vector<game_msg> PlayersSettingsWindow::gather_info()
{
    std::vector<game_msg> res;
    for (int i = 0; i < in_set.size(); i++){
        game_msg msg = in_set[i]->get_info_msg();
        msg.target_id = i;
        res.push_back(msg);
    }

    return res;
}

void PlayersSettingsWindow::check_all_ready()
{
    for(const auto& i : players_ready)
        if(!i)
        {
            start_the_game->setEnabled(0);
            return;
        }
    start_the_game->setEnabled(1);
}

void PlayersSettingsWindow::set_connected(int id, bool val)
{
    in_set[id]->set_connected(val);
}

void PlayersSettingsWindow::someone_connected(int id)
{
    in_set[id]->setEnabled(false);
    in_set[id]->set_connected(true);

    if (Transceiver::get_transceiver()->get_id() == 0)
        check_all_ready();
}

void PlayersSettingsWindow::someone_disconnected(int id)
{
    in_set[id]->set_connected(false);
    in_set[id]->clear();
    players_ready[id] = false;
}

void PlayersSettingsWindow::update_access()
{
    qDebug() << Transceiver::get_transceiver()->get_id();
    for (int i = 0; i < in_set.size(); i++){
        if (i != Transceiver::get_transceiver()->get_id())
            in_set[i]->setEnabled(false);
    }
}

void PlayersSettingsWindow::get_info_msg(game_msg msg)
{
    in_set[msg.target_id]->update_info(msg);

    players_ready[msg.target_id] = msg.extra;


    if (Transceiver::get_transceiver()->get_id() == 0)
        check_all_ready();
}

void PlayersSettingsWindow::sync(int id)
{
    emit sync_data(gather_info(), id);
}



void PlayersSettingsWindow::clear()
{
    for (const auto& i : in_set)
        i->clear();
    for (auto& i : data)
        i = {"", ""};
    for (auto& i : players_ready)
        i = 0;
}

void PlayersSettingsWindow::set_player(int id, bool is_ready, std::string name, std::string stats)
{
    data[id] = {name, stats};
    players_ready[id] = is_ready;

    if (Transceiver::get_transceiver()->get_id() == 0)
        check_all_ready();
}

void PlayersSettingsWindow::start_button_is_clicked()
{
    close();
    emit all_are_ready(data);
}



