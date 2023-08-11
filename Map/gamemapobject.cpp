#include "gamemapobject.h"

GameMapObject::GameMapObject(int width, int height, QBrush brush) :
   QGraphicsItem()
{
    this->width = width;
    this->height = height;
    this->brush = brush;
    is_hovered = false;
    setAcceptHoverEvents(true);
}

bool GameMapObject::get_hovered() const
{
    return is_hovered;
}

QRect GameMapObject::get_position() const
{
    return QRect(0, 0, width, height);
}

QRectF GameMapObject::boundingRect() const
{
    return QRectF(0, 0, width, height);
}

