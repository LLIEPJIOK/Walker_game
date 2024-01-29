
#include "congratulationwindow.h"
#include <QPainter>
#include <QPixmap>
CongratulationWindow::CongratulationWindow(QWidget *parent, QString winner)
    : QWidget{parent}
{
    setFixedSize(parent->size());
    QFont font ("Arial", 14, QFont::Normal, 1);
    QString style("color: rgb(255, 255, 255)");
    exit_button = new QPushButton(this);
    exit_button->setText(tr("Exit"));
    exit_button->setFont(font);
    exit_button->setFlat(1);
    exit_button->setStyleSheet(style);
    connect(exit_button, &QPushButton::clicked, this, &CongratulationWindow::exit_the_game);


    this->winner = new QLabel(this);
    this->winner->setText(tr("Congratulations, ") + winner);
    this->winner->setFont(QFont("Arial", 30, QFont::Normal, 1));
    this->winner->setStyleSheet(style);

    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    layout->addWidget(this->winner);
    layout->addWidget(exit_button);
    setLayout(layout);

}

void CongratulationWindow::paintEvent(QPaintEvent *event)
{

    QPixmap background(":/end/Pictures/End/Win.png");
    background = background.scaled(this->width(), this->height());
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, this->width(), this->height());
    painter.drawPixmap(0, 0, background);
    QWidget::paintEvent(event);
}

