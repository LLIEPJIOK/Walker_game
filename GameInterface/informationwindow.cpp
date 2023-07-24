#include "informationwindow.h"

InformationWindow::InformationWindow(QWidget *parent)
    : QFrame{parent}
{
    setStyleSheet("background: transparent");

    opacity = 0;

    auto size = QApplication::screens().at(0)->size();
    setGeometry(size.width() - 205, size.height() - 55, 200, 50);

    label = new QLabel(this);
    label->setFont(QFont("Arial", 14));
    label->setAlignment(Qt::AlignCenter);
    label->resize(width(), 50);
    label->setStyleSheet("QLabel {color: white;}");

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &InformationWindow::timer_tick);
}

void InformationWindow::inform(QString str)
{
    label->setText(str);
    opacity = 255;
    timer->start(50);
    show();
}

void InformationWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, opacity));
    painter.drawRoundedRect(0, 0, width(), height(), 5, 5);
}

void InformationWindow::timer_tick()
{
    if (opacity <= 0)
    {
        hide();
        return;
    }

    label->setStyleSheet("QLabel {color: rgba(255, 255, 255, " + QString::number(opacity) + ");}");
    opacity -= 3;
    repaint();
}
