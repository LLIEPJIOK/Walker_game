#include "movingarea.h"
#include "minimap.h"

MovingArea::MovingArea(QWidget *parent)
    : QFrame{parent}
{

}

void MovingArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        old_pos = event->pos();
    }
}

void MovingArea::mouseMoveEvent(QMouseEvent *event)
{
    MiniMap* mini_map = dynamic_cast<MiniMap*>(qobject_cast<QWidget*>(parent()));
    QPoint delta = event->pos() - old_pos;
    QPoint to_move = pos() + delta;

    if(to_move.x() < 0)
        to_move.setX(0);
    if(to_move.y() < 0)
        to_move.setY(0);
    if (to_move.x() > mini_map->width() - width())
        to_move.setX(mini_map->width() - width());
    if (to_move.y() > mini_map->height() - height())
        to_move.setY(mini_map->height() - height());
    move(to_move);

    auto hex_map = mini_map->get_connected_hex_map();
    to_move.setX(hex_map->get_source_pixmap_size().width() * to_move.x() / mini_map->width());
    to_move.setY(hex_map->get_source_pixmap_size().height() * to_move.y() / mini_map->height());
    hex_map->update_current_area(to_move);
}
