#include "mapobject.h"

MapObject::MapObject(int width, int height) :
    QGraphicsItem()
{
    this->width = width;
    this->height = height;
    is_hovered = false;
    setAcceptHoverEvents(true);
}

bool MapObject::get_hovered() const
{
    return is_hovered;
}

QRectF MapObject::boundingRect() const
{
    return QRectF(0, 0, width, height);
}
