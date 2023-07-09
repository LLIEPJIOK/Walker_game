#include "titers.h"

Titers::Titers(QWidget *parent)
    : QGraphicsView(parent)
{
    painter = new QPainter();

    QPalette pal = palette();
    pal.setColor(QPalette::Active, QPalette::Window, Qt::black);

    setPalette(pal);

    size = QApplication::screens().at(0)->size();
    y = size.height();

    tt = new TitersText();

    gs = new QGraphicsScene(this);
    gs->setBackgroundBrush(Qt::black);
    gs->addItem(tt);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameStyle(QFrame::Panel);

    setScene(gs);
    setSceneRect(QRect(QPoint(0, 0), size));
    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Titers::sdvig);
    connect(tt, &TitersText::open_menu_signal, this, &Titers::open_menu_signal);
    timer->start(10);
}

void Titers::reset()
{
    tt->reset();
}

Titers::~Titers()
{
    delete tt;
}

void Titers::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit open_menu_signal();
}

void Titers::sdvig()
{
    tt->moveBy(0, -1);
}
