#include "cell.h"
#include <QKeyEvent>
Cell::Cell(QObject *parent, int width, int height, QString tile_name) :
    QObject(parent), GameMapObject(width, height)
{
    tile.load("../Game/Resources/Pictures/Tiles/" + tile_name+ "8.png");
    tile = tile.copy(64, 16, 32, 32);
    tile = tile.scaled(width, height);
    is_way = false;
}

void Cell::highlight()
{
    is_way = true;
    update();
}

void Cell::normalize()
{
    is_way = false;
    update();
}

void Cell::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    is_hovered = true;
    emit cell_signal();
    update();
}

void Cell::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    is_hovered = false;
    emit cell_signal();
    update();
}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && is_way)
        emit way_to_go();
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPixmap(0, 0, tile);
    QPen pen;
    if(is_way)
    {
        pen.setColor(QColorConstants::Svg::coral);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawRect(0, 0, width, height);
    }
    else if (is_hovered)
    {
        pen.setColor(Qt::red);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawRect(0, 0, width, height);
    }


}

