
#include "movingarea.h"
MovingArea::MovingArea(QWidget *parent, GameMap *battle_map)
    : QFrame{parent}
{
    this->battle_map = battle_map;
}

void MovingArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        old_pos = event->pos();
    }
}

void MovingArea::mouseMoveEvent(QMouseEvent *event)
{
    QWidget* p = qobject_cast<QWidget*>(parent());
    QPoint delta = event->pos() - old_pos;
    QPoint to_move = pos() + delta;
    if(pos().x() + delta.x() < 0)
        to_move.setX(0);
    if(pos().y() + delta.y() < 0)
        to_move.setY(0);
    if (pos().x() + delta.x() > p->width() - width())
        to_move.setX(p->width() - width() - 1);
    if (pos().y() + delta.y() > p->height() - height())
        to_move.setY(p->height() - height() - 1);
    move(to_move);
    battle_map->update_current_area(10 * to_move);
}
