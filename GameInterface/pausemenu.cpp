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

    continue_button = new QPushButton(tr("Continue"));
    continue_button->setMinimumSize(300, 50);
    continue_button->setFont(font);
    continue_button->setFlat(1);

    save_menu = new QPushButton(tr("Save"));
    save_menu->setMinimumSize(300, 50);
    save_menu->setFont(font);
    save_menu->setFlat(1);

    main_menu = new QPushButton(tr("Main menu"));
    main_menu->setMinimumSize(300, 50);
    main_menu->setFont(font);
    main_menu->setFlat(1);

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

