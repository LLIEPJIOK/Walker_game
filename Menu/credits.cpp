#include "credits.h"

Credits::Credits(QWidget *parent)
    : QGraphicsView(parent)
{
    painter = new QPainter();

    QPalette pal = palette();
    pal.setColor(QPalette::Active, QPalette::Window, Qt::black);

    setPalette(pal);

    size = QApplication::screens().at(0)->size();
    y = size.height();

    tt = new CreditsText();

    gs = new QGraphicsScene(this);
    gs->setBackgroundBrush(Qt::black);
    gs->addItem(tt);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameStyle(QFrame::Panel);

    setScene(gs);
    setSceneRect(QRect(QPoint(0, 0), size));
    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Credits::sdvig);
    connect(tt, &CreditsText::open_menu_signal, this, &Credits::open_menu_signal);
    timer->start(10);
}

void Credits::reset()
{
    tt->reset();
}

Credits::~Credits()
{
    delete tt;
}

void Credits::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit open_menu_signal();
}

void Credits::sdvig()
{
    tt->moveBy(0, -1);
}
