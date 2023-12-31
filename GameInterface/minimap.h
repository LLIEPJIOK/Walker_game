#ifndef MINIMAP_H
#define MINIMAP_H

#include "movingarea.h"

#include <QFrame>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDragMoveEvent>
#include <QApplication>

class MiniMap : public QFrame
{
    Q_OBJECT
public:
    explicit MiniMap(QWidget *parent = nullptr, GameMap* battle_map = nullptr);
private:
    GameMap* battle_map;
    MovingArea *moving_area;
    void paintEvent(QPaintEvent *event);
    QPair<int, int> battle_map_size;

private slots:
    void sync_moving_area(QPoint position);

};

#endif // MINIMAP_H
