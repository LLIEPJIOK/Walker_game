#ifndef MAPOBJECT_H
#define MAPOBJECT_H


#include <QWidget>
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>

class MapObject : public QGraphicsItem
{
public:
    explicit MapObject(int width = 0, int height = 0);
    bool get_hovered() const;
    QRectF boundingRect() const;
protected:
    int width, height;
    bool is_hovered;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) = 0;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) = 0;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) = 0;
signals:

};

#endif // MAPOBJECT_H
