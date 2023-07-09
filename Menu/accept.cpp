#include "accept.h"

Accept::Accept(QString str, QWidget *parent)
    : QFrame(parent)
{
    auto size = QApplication::screens().at(0)->size();
    setGeometry(size.width() / 2. - 200, (size.height() - 125) / 2., 400, 125);

    setStyleSheet("background: rgb(180, 180, 180)");
    setAutoFillBackground(1);

    label = new QLabel(str, this);
    label->setFont(QFont("Arial", 16));
    label->setAlignment(Qt::AlignCenter);
    label->resize(width(), 50);
    label->move(0, 15);

    btn_yes = new QPushButton("Да", this);
    btn_yes->setFlat(1);
    btn_yes->setFont(QFont("Arial", 14, QFont::Normal, 1));
    btn_yes->resize(50, 30);
    btn_yes->move(25, 75);

    btn_no = new QPushButton("Нет", this);
    btn_no->setFlat(1);
    btn_no->setFont(QFont("Arial", 14, QFont::Normal, 1));
    btn_no->resize(50, 30);
    btn_no->move(width() - 75, 75);

    connect(btn_yes, SIGNAL(clicked()), this, SLOT(return_slot()));
    connect(btn_no, SIGNAL(clicked()), this, SLOT(return_slot()));
}

void Accept::return_slot()
{
    emit accept_signal(sender() == btn_yes);
}
