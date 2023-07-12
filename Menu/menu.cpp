#include "menu.h"
#include <QPixmap>
#include <QStyle>
Menu::Menu(QWidget *parent) : QMainWindow(parent)
{
    Q_UNUSED(parent);

    QFont font ("Arial", 14, QFont::Normal, 1);
    QString style("color: rgb(255, 255, 255)");
    btn_new_game = new QPushButton("Новая игра");
    btn_new_game->setMinimumSize(300, 50);
    btn_new_game->setFlat(1);
    btn_new_game->setFont(font);
    btn_new_game->setStyleSheet(style);

    btn_load = new QPushButton("Загрузить");
    btn_load->setMinimumSize(300, 50);
    btn_load->setFlat(1);
    btn_load->setFont(font);
    btn_load->setStyleSheet(style);

    btn_titers = new QPushButton("Об игре");
    btn_titers->setMinimumSize(300, 50);
    btn_titers->setFlat(1);
    btn_titers->setFont(font);
    btn_titers->setStyleSheet(style);

    btn_exit = new QPushButton("Выход");
    btn_exit->setMinimumSize(300, 50);
    btn_exit->setFlat(1);
    btn_exit->setFont(font);
    btn_exit->setStyleSheet(style);

    vblay = new QVBoxLayout();
    vblay->setAlignment(Qt::AlignCenter);
    vblay->setSpacing(10);
    vblay->addWidget(btn_new_game);
    vblay->addWidget(btn_load);
    vblay->addWidget(btn_titers);
    vblay->addWidget(btn_exit);

    hblay = new QHBoxLayout();
    hblay->setAlignment(Qt::AlignCenter);
    hblay->addLayout(vblay);

    widget = new QWidget();
    widget->setLayout(hblay);
    setCentralWidget(widget);

    new_game = new NewGame();
    titers = new Titers();
    exit_window = new ExitWindow();

    connect(btn_new_game, SIGNAL(clicked()), this, SLOT(open_new_game()));
    connect(btn_load, SIGNAL(clicked()), this, SIGNAL(load_the_game()));
    connect(btn_titers, SIGNAL(clicked()), this, SLOT(open_titers()));
    connect(btn_exit, SIGNAL(clicked()), this, SLOT(open_exit_window()));

    connect(new_game, &NewGame::open_menu_signal, this, &Menu::menu_enable);
    connect(titers, &Titers::open_menu_signal, this, &Menu::menu_enable);
    connect(exit_window, &ExitWindow::signal_open_menu, this, &Menu::menu_enable);
}

Menu::~Menu()
{
    delete new_game;
    delete titers;
    delete exit_window;
}

void Menu::open_exit_window()
{
    centralWidget()->setParent(0);
    setCentralWidget(exit_window);
}

void Menu::open_titers()
{
    centralWidget()->setParent(0);
    titers->reset();
    setCentralWidget(titers);
}

void Menu::open_new_game()
{
    centralWidget()->setParent(0);
    setCentralWidget(new_game);
}

void Menu::menu_enable()
{
    centralWidget()->setParent(0);
    setCentralWidget(widget);
}

void Menu::paintEvent(QPaintEvent *event)
{
    QPixmap background("../Game/Resources/Pictures/Background.png");
    background = background.scaled(this->width(), this->height());
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);
    QMainWindow::paintEvent(event);
}