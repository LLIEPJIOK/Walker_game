
#include "pausemenu.h"
#include <QPixmap>

PauseMenu::PauseMenu(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(parent->size());
    QFont font ("Arial", 14, QFont::Normal, 1);
    QString style("color: rgb(255, 255, 255)");

    continue_button = new QPushButton(this);
    continue_button->setText("Продолжить");
    continue_button->setFont(font);
    continue_button->setFlat(1);
    continue_button->setStyleSheet(style);

    main_menu = new QPushButton(this);
    main_menu->setText("Главное меню");
    main_menu->setFont(font);
    main_menu->setFlat(1);
    main_menu->setStyleSheet(style);

    vblay = new QVBoxLayout(this);
    vblay->setAlignment(Qt::AlignCenter);
    vblay->addWidget(continue_button);
    vblay->addWidget(main_menu);

    connect(continue_button, &QPushButton::clicked, this, &PauseMenu::continue_button_clicked);
    connect(main_menu, &QPushButton::clicked, this, &PauseMenu::main_menu_clicked);
}

void PauseMenu::paintEvent(QPaintEvent *event)
{
    QPixmap background("../Game/Resources/Pictures/Background.png");
    background = background.scaled(this->width(), this->height());
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);
    QWidget::paintEvent(event);
}

