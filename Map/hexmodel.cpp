#include "hexmodel.h"

HexModel::HexModel(const Coordinates::Hex<int>& hex_) : QObject(nullptr), MapObject(0, 0) {
    hex = hex_;
    auto hex_corners = HexMathOnScreen::corners(HexMathOnScreen::hexToPixel(hex));
    for(const auto& i : hex_corners) {
        polygon << i;
    }

    is_way = false;
}

void HexModel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    painter->setBrush(Qt::darkGreen);

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
    painter->drawPolygon(polygon);
}

void HexModel::set_way(bool way)
{
    is_way = way;
    update();
}

bool HexModel::get_way() const
{
    return is_way;
}

void HexModel::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    is_hovered = true;
    update();
}

void HexModel::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    is_hovered = false;
    update();
}

void HexModel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && is_way)
        emit was_chosen_as_way(hex);
}

QPainterPath HexModel::shape() const
{
    QPainterPath path;
    path.addPolygon(polygon);
    return path;
}

QRectF HexModel::boundingRect() const
{
    return polygon.boundingRect();
}

