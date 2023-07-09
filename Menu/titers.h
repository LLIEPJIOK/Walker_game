#ifndef TITERS_H
#define TITERS_H

#include "titerstext.h"

#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>

class Titers : public QGraphicsView
{
    Q_OBJECT
public:
    Titers(QWidget *parent = nullptr);
    void reset();
    ~Titers();
private:
    int my_timer;
    qreal y;
    QLabel *label;
    QPainter *painter;
    QSize size;
    TitersText *tt;
    QGraphicsScene * gs;
protected:
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void open_menu_signal();
public slots:
    void sdvig();
};

#endif // TITERS_H
