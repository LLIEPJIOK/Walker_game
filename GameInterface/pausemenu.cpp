#include "pausemenu.h"

PauseMenu::PauseMenu(QMainWindow *parent)
    : QMainWindow(parent)
{
    setWindowFlags(windowFlags() ^ Qt::Popup);
    setFixedSize(parent->size());
    QFont font ("Arial", 14, QFont::Normal, 1);

    load = new Load("save");

    continue_button = new QPushButton();
    continue_button->setText("Продолжить");
    continue_button->setFont(font);
    continue_button->setFlat(1);
    continue_button->setStyleSheet("QPushButton        {color: white;}"
                                   "QPushButton:hover  {color: rgb(255, 178, 102);}");

    save_menu = new QPushButton("Сохранить");
    save_menu->setFont(font);
    save_menu->setFlat(1);
    save_menu->setStyleSheet("QPushButton        {color: white;}"
                             "QPushButton:hover  {color: rgb(255, 178, 102);}");

    main_menu = new QPushButton();
    main_menu->setText("Главное меню");
    main_menu->setFont(font);
    main_menu->setFlat(1);
    main_menu->setStyleSheet("QPushButton        {color: white;}"
                             "QPushButton:hover  {color: rgb(255, 178, 102);}");

    vblay = new QVBoxLayout();
    vblay->setAlignment(Qt::AlignCenter);
    vblay->addWidget(continue_button);
    vblay->addWidget(save_menu);
    vblay->addWidget(main_menu);

    pause_menu = new QWidget();
    pause_menu->setLayout(vblay);
    setCentralWidget(pause_menu);

    connect(continue_button, &QPushButton::clicked, this, &PauseMenu::continue_button_clicked);
    connect(main_menu, &QPushButton::clicked, this, &PauseMenu::main_menu_clicked);
    connect(save_menu, SIGNAL(clicked()), this, SLOT(open_save()));
    connect(load, &Load::save_game, this, &PauseMenu::save_game_slot);
    connect(load, &Load::return_back_signal, this, &PauseMenu::show_pause);

}

void PauseMenu::paintEvent(QPaintEvent *event)
{
    QPixmap background("../Game/Resources/Pictures/Background.png");
    background = background.scaled(this->width(), this->height());
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);
    QWidget::paintEvent(event);
}

void PauseMenu::save_game_slot(QString file_name)
{
    centralWidget()->setParent(nullptr);
    setCentralWidget(pause_menu);
    hide();
    emit save_game_signal(file_name);
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

