#include "playerssettingswindow.h"
#include "general.h"

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

    start_the_game = new QPushButton("Начать");
    start_the_game->setFlat(1);
    start_the_game->setEnabled(0);
    start_the_game->setFont(btn_font);

    QPalette pal = start_the_game->palette();
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::gray);
    start_the_game->setPalette(pal);

    back = new QPushButton("Назад");
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
    check_all_ready();
}

void PlayersSettingsWindow::start_button_is_clicked()
{
    close();
    emit all_are_ready(data);
}



