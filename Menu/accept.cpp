#include "accept.h"

Accept::Accept(QString str, QWidget *parent)
    : QFrame(parent)
{
    auto size = QApplication::screens().at(0)->size();
    setGeometry(size.width() / 2 - 250, size.height() / 2 - 105, 500, 210);


    auto btn_font = QFont("Arial", 14, QFont::Normal, 1);
    setStyleSheet("QPushButton        {color: white;}"
                  "QPushButton:hover  {color: rgb(255, 178, 102);}"
                  "QLabel             {color: white;}");

    label = new QLabel(str, this);
    label->setFont(QFont("Arial", 16));
    label->setAlignment(Qt::AlignCenter);
    label->resize(width(), 70);
    label->move(0, 35);

    btn_yes = new QPushButton(tr("Yes"), this);
    btn_yes->setFlat(1);
    btn_yes->setFont(btn_font);
    btn_yes->resize(50, 30);
    btn_yes->move(25, 125);

    btn_no = new QPushButton(tr("No"), this);
    btn_no->setFlat(1);
    btn_no->setFont(btn_font);
    btn_no->resize(50, 30);
    btn_no->move(width() - 75, 125);

    connect(btn_yes, SIGNAL(clicked()), this, SLOT(return_slot()));
    connect(btn_no, SIGNAL(clicked()), this, SLOT(return_slot()));
}

void Accept::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 220));
    painter.drawRect(0, 0, width(), height());
}

void Accept::return_slot()
{
    emit accept_signal(sender() == btn_yes);
}
