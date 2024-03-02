#include "menu.h"
#include "qtranslator.h"
#include "Engine/translator.h"
#include "Engine/Transceiver.h"
#include "joingamewidget.h"

#include <QPixmap>
#include <QStyle>

Menu::Menu(QWidget *parent) : QMainWindow(parent)
{
    eng = (translator.language() == "en") || translator.isEmpty();
    QFont btn_font ("Arial", 14, QFont::Normal, 1);
    setStyleSheet("QPushButton        {color: white;}"
                  "QPushButton:hover  {color: rgb(255, 178, 102);}");

    QVector<QPushButton*> tmp;

    for (int i = 0 ; i < 5; i++){
        tmp.push_back(new QPushButton());
        tmp.back()->setMinimumSize(300, 50);
        tmp.back()->setFlat(1);
        tmp.back()->setFont(btn_font);
    }

//    btn_new_game = tmp[0];
//    btn_new_game->setText(tr("New game"));
//    btn_load = tmp[1];
//    btn_load->setText(tr("Load"));
//    btn_titers = tmp[2];

    btn_lobby = tmp[0];
    btn_lobby->setText(tr("Create Lobby"));
    btn_connect = tmp[1];
    btn_connect->setText(tr("Join Lobby"));
    btn_titers = tmp[2];
    btn_titers->setText(tr("About"));
    btn_exit = tmp[3];
    btn_exit->setText(tr("Exit"));
    btn_lang = tmp[4];
    btn_lang->setText(tr("English"));

    vblay = new QVBoxLayout();
    vblay->setAlignment(Qt::AlignCenter);
    vblay->setSpacing(10);
    vblay->addWidget(btn_lobby);
    vblay->addWidget(btn_connect);
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

    connect(btn_lobby, SIGNAL(clicked()), this, SLOT(open_new_game()));
    connect(btn_connect, SIGNAL(clicked()), this, SLOT(connectTo()));
    connect(btn_titers, SIGNAL(clicked()), this, SLOT(open_titers()));
    connect(btn_exit, SIGNAL(clicked()), this, SLOT(open_exit_window()));
    connect(btn_lang, SIGNAL(clicked()), this, SLOT(change_lang()));

    connect(load, &Load::return_back_signal, this, &Menu::menu_enable);
    connect(new_game, &NewGame::open_menu_signal, this, &Menu::menu_enable);
    connect(titers, &Credits::open_menu_signal, this, &Menu::menu_enable);
    connect(exit_window, &ExitWindow::signal_open_menu, this, &Menu::menu_enable);

    //connect(Transceiver::get_transceiver(), &Transceiver::connect_successful, new_game, &NewGame::go_choose_players);
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
        if (!translator.load(":/files/Files/Game_ru.qm"))
            qDebug("Translation to ru_RU has failed");

        QApplication::installTranslator(&translator);
        Translator::load(":/files/Files/Translator_ru.txt");
        eng = false;
    }
    else{
        if (!translator.load(":/files/Files/Game_en.qm"))
            qDebug("Translation to en_EN has failed");

        Translator::load(":/files/Files/Translator_en.txt");
        QApplication::installTranslator(&translator);
        eng = true;
    }

    update_lang();
    new_game->update_lang();
    exit_window->update_lang();
    load->update_lang();

}

void Menu::connectTo()
{
    QWidget * wid = new QWidget;
    QGridLayout * layout = new QGridLayout(wid);
    JoinGameWidget* w = new JoinGameWidget(wid);
    layout->addWidget(w);
    layout->setAlignment(w, Qt::AlignCenter);

    connect(w, &JoinGameWidget::cancel, this, &Menu::menu_enable);
    connect(w, &JoinGameWidget::cancel, wid, &QObject::deleteLater);
    connect(Transceiver::get_transceiver(), &Transceiver::join_successful, wid, &QObject::deleteLater);
    connect(w, &JoinGameWidget::joinTo, Transceiver::get_transceiver(), &Transceiver::connectTo);
    centralWidget()->setParent(0);
    setCentralWidget(wid);
    wid->show();
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
//    btn_new_game->setText(tr("New game"));
//    btn_load->setText(tr("Load"));
    btn_lobby->setText(tr("Create Lobby"));
    btn_connect->setText(tr("Join Lobby"));
    btn_titers->setText(tr("About"));
    btn_exit->setText(tr("Exit"));
    btn_lang->setText(tr("English"));
}

QTranslator Menu::translator;
