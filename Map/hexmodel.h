#ifndef HEXMODEL_H
#define HEXMODEL_H

#include "mapobject.h"
#include "Hex.h"
#include <QPolygonF>
#include <QObject>

class HexModel : public QObject, public MapObject
{
    Q_OBJECT
private:
    QPolygonF polygon;
    bool is_way;
    Coordinates::Hex<int> hex;
public:
    HexModel(const Coordinates::Hex<int>& hex_);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void set_way(bool way);
    bool get_way() const;
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    QPainterPath shape() const override;
    QRectF boundingRect() const override;
signals:
    void was_chosen_as_way(const Coordinates::Hex<int>& hex);
};

#endif // HEXMODEL_H
