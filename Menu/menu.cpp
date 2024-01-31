#include "menu.h"
#include "qtranslator.h"

#include <QPixmap>
#include <QStyle>

Menu::Menu(QWidget *parent) : QMainWindow(parent)
{
    eng = true;
    translator = new QTranslator();
    translator->load("D:/Game/Game/Game_en");
    QFont btn_font ("Arial", 14, QFont::Normal, 1);
    setStyleSheet("QPushButton        {color: white;}"
                  "QPushButton:hover  {color: rgb(255, 178, 102);}");

    btn_new_game = new QPushButton(tr("New game"));
    btn_new_game->setMinimumSize(300, 50);
    btn_new_game->setFlat(1);
    btn_new_game->setFont(btn_font);

    btn_load = new QPushButton(tr("Load"));
    btn_load->setMinimumSize(300, 50);
    btn_load->setFlat(1);
    btn_load->setFont(btn_font);

    btn_titers = new QPushButton(tr("About"));
    btn_titers->setMinimumSize(300, 50);
    btn_titers->setFlat(1);
    btn_titers->setFont(btn_font);

    btn_exit = new QPushButton(tr("Exit"));
    btn_exit->setMinimumSize(300, 50);
    btn_exit->setFlat(1);
    btn_exit->setFont(btn_font);

    btn_lang = new QPushButton(tr("English"));
    btn_lang->setMinimumSize(300, 50);
    btn_lang->setFlat(1);
    btn_lang->setFont(btn_font);

    vblay = new QVBoxLayout();
    vblay->setAlignment(Qt::AlignCenter);
    vblay->setSpacing(10);
    vblay->addWidget(btn_new_game);
    vblay->addWidget(btn_load);
    vblay->addWidget(btn_lang);
    vblay->addWidget(btn_titers);
    vblay->addWidget(btn_exit);

    widget = new QWidget();
    widget->setLayout(vblay);
    setCentralWidget(widget);

    new_game = new NewGame();
    titers = new Credits();
    exit_window = new ExitWindow();
    load = new Load("load");

    connect(btn_new_game, SIGNAL(clicked()), this, SLOT(open_new_game()));
    connect(btn_load, SIGNAL(clicked()), this, SLOT(open_load()));
    connect(btn_titers, SIGNAL(clicked()), this, SLOT(open_titers()));
    connect(btn_exit, SIGNAL(clicked()), this, SLOT(open_exit_window()));
    connect(btn_lang, SIGNAL(clicked()), this, SLOT(change_lang()));

    connect(load, &Load::return_back_signal, this, &Menu::menu_enable);
    connect(new_game, &NewGame::open_menu_signal, this, &Menu::menu_enable);
    connect(titers, &Credits::open_menu_signal, this, &Menu::menu_enable);
    connect(exit_window, &ExitWindow::signal_open_menu, this, &Menu::menu_enable);
}

Menu::~Menu()
{
    delete new_game;
    delete titers;
    delete exit_window;
    delete load;
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

void Menu::open_load()
{
    centralWidget()->setParent(0);
    setCentralWidget(load);
}

void Menu::menu_enable()
{
    centralWidget()->setParent(0);
    setCentralWidget(widget);
}

void Menu::change_lang()
{
    if (eng){
        translator->load("D:/Game/Game/Game_ru");
        QApplication::installTranslator(translator);
        eng = false;
    }
    else{
        translator->load("D:/Game/Game/Game_en");
        QApplication::installTranslator(translator);
        eng = true;
    }

    update_lang();
    new_game->update_lang();
    exit_window->update_lang();
    load->update_lang();

}

void Menu::paintEvent(QPaintEvent *event)
{
    QPixmap background(":/backgrounds/Pictures/widget_backgrounds/Background.png");
    background = background.scaled(this->width(), this->height());
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);
    QMainWindow::paintEvent(event);
}

void Menu::update_lang()
{
    btn_new_game->setText(tr("New game"));
    btn_load->setText(tr("Load"));
    btn_titers->setText(tr("About"));
    btn_exit->setText(tr("Exit"));
    btn_lang->setText(tr("English"));
}
