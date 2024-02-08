#include "pausemenu.h"

PauseMenu::PauseMenu(QMainWindow *parent)
    : QMainWindow(parent)
{
    setWindowFlags(windowFlags() ^ Qt::Popup);
    setFixedSize(parent->size());

    QFont font ("Arial", 14, QFont::Normal, 1);
    setStyleSheet("QPushButton        {color: white;}"
                  "QPushButton:hover  {color: rgb(255, 178, 102);}");

    load = new Load("save");

    QVector<QPushButton*> tmp;

    for (int i = 0; i < 3; i++){
        tmp.push_back(new QPushButton);
        tmp.back()->setMinimumSize(300, 50);
        tmp.back()->setFont(font);
        tmp.back()->setFlat(1);
    }

    continue_button = tmp[0];
    tmp[0]->setText("Continue");

    save_menu = tmp[1];
    tmp[1]->setText("Save");

    main_menu = tmp[2];
    tmp[2]->setText("Main menu");

    vblay = new QVBoxLayout();
    vblay->setAlignment(Qt::AlignCenter);
    vblay->setSpacing(10);
    vblay->addWidget(continue_button);
    vblay->addWidget(save_menu);
    vblay->addWidget(main_menu);

    pause_menu = new QWidget();
    pause_menu->setLayout(vblay);
    setCentralWidget(pause_menu);

    connect(continue_button, &QPushButton::clicked, this, &PauseMenu::continue_button_clicked_slot);
    connect(main_menu, &QPushButton::clicked, this, &PauseMenu::main_menu_clicked);
    connect(save_menu, SIGNAL(clicked()), this, SLOT(open_save()));
    connect(load, &Load::save_game, this, &PauseMenu::save_game_slot);
    connect(load, &Load::return_back_signal, this, &PauseMenu::show_pause);
}

void PauseMenu::paintEvent(QPaintEvent *event)
{
    QPixmap background(":/backgrounds/Pictures/widget_backgrounds/Background.png");
    background = background.scaled(this->width(), this->height());
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);
    QWidget::paintEvent(event);
}

void PauseMenu::save_game_slot(QString file_name)
{
    show_pause();
    load->reset();
    emit save_game_signal(file_name);
}

void PauseMenu::continue_button_clicked_slot()
{
    load->reset();
    emit continue_button_clicked_signal();
}

void PauseMenu::open_save()
{
    centralWidget()->setParent(nullptr);
    setCentralWidget(load);
}

void PauseMenu::show_pause()
{
    centralWidget()->setParent(nullptr);
    setCentralWidget(pause_menu);
}

