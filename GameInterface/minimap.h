
#ifndef MINIMAP_H
#define MINIMAP_H


#include <QFrame>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDragMoveEvent>
#include <QApplication>
#include "movingarea.h"

class MiniMap : public QFrame
{
    Q_OBJECT
public:
    explicit MiniMap(QWidget *parent = nullptr, GameMap* battle_map = nullptr);
private:
    GameMap* battle_map;
    MovingArea *moving_area;
    void paintEvent(QPaintEvent *event);

signals:

};

#endif // MINIMAP_H
