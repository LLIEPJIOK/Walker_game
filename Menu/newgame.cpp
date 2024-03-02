#include "newgame.h"

#include "Engine/Transceiver.h"

NewGame::NewGame(QWidget *parent) : QMainWindow(parent)
{
    QFont btn_font ("Arial", 14, QFont::Normal, 1);
    setStyleSheet("QPushButton:active   {color: white;}"
                  "QPushButton:hover    {color: rgb(255, 178, 102);}"
                  "QLabel               {color: white;}");

    label_choose = new QLabel(tr("Quantity of players"));
    label_choose->setFont(QFont("Arial", 16, QFont::Normal, 1));

    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(2);
    slider->setMaximum(4);
    slider->setFixedSize(300, 20);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(1);

    label_2 = new QLabel("2");
    label_2->setAlignment(Qt::AlignTop);

    label_3 = new QLabel("3");
    label_3->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    label_4 = new QLabel("4");
    label_4->setAlignment(Qt::AlignRight | Qt::AlignTop);

    label_hblay = new QHBoxLayout();
    label_hblay->addWidget(label_2);
    label_hblay->addWidget(label_3);
    label_hblay->addWidget(label_4);

    gridlay = new QGridLayout();
    gridlay->setSpacing(0);
    gridlay->addWidget(slider, 0, 0);
    gridlay->addLayout(label_hblay, 1, 0);

    btn_next = new QPushButton();
    btn_next->setFlat(1);
    btn_next->setText(tr("Continue"));
    btn_next->setFont(btn_font);

    btn_prev = new QPushButton();
    btn_prev->setFlat(1);
    btn_prev->setText(tr("Return"));
    btn_prev->setFont(btn_font);

    btn_hblay = new QHBoxLayout();
    btn_hblay->addWidget(btn_prev);
    btn_hblay->addWidget(btn_next);
    btn_hblay->setSpacing(QApplication::screens().at(0)->size().width() - 300);

    vblay = new QVBoxLayout();
    vblay->setSpacing(30);
    vblay->addWidget(label_choose);
    vblay->addLayout(gridlay);
    vblay->setAlignment(Qt::AlignCenter);

    hblay = new QHBoxLayout();
    hblay->addLayout(vblay);
    hblay->setAlignment(Qt::AlignCenter);

    all_vblay = new QVBoxLayout();
    all_vblay->setAlignment(Qt::AlignJustify);
    all_vblay->addLayout(hblay);
    all_vblay->addLayout(btn_hblay);
    all_vblay->setContentsMargins(0, 0, 0, 30);

    widget = new QWidget();
    widget->setLayout(all_vblay);
    setCentralWidget(widget);

    player_set_window = 0;

    connect(btn_prev, SIGNAL(clicked()), this, SIGNAL(open_menu_signal()));
    connect(btn_prev, SIGNAL(clicked()), Transceiver::get_transceiver(), SLOT(terminate()));
    connect(btn_next, SIGNAL(clicked()), this, SLOT(go_choose_players()));
    connect(this, &NewGame::start_lobby, Transceiver::get_transceiver(), &Transceiver::startListening);
}

NewGame::~NewGame()
{
    delete player_set_window;
}

void NewGame::update_lang()
{
    btn_next->setText(tr("Continue"));
    btn_prev->setText(tr("Return"));
    label_choose->setText(tr("Quantity of players"));
    if (player_set_window != nullptr)
        player_set_window->update_lang();

}

void NewGame::go_choose_players()
{
    emit start_lobby(slider->value() -1);
    if (player_set_window)
        delete player_set_window;

    player_set_window = new PlayersSettingsWindow(slider->value());
    connect(player_set_window, &PlayersSettingsWindow::go_back, this, &NewGame::show_new_game);

    centralWidget()->setParent(0);
    setCentralWidget(player_set_window);
}

void NewGame::show_new_game()
{
    centralWidget()->setParent(0);
    setCentralWidget(widget);
}
