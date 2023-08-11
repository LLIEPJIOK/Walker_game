
#ifndef GAMEMAPOBJECT_H
#define GAMEMAPOBJECT_H


#include <QWidget>
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>
class GameMapObject : public QGraphicsItem
{
public:
    explicit GameMapObject(int width = 0, int height = 0, QBrush brush = QBrush(Qt::NoBrush));
    bool get_hovered() const;
    QRect get_position() const;
    QRectF boundingRect() const;
protected:
    int width, height;
    QBrush brush;
    bool is_hovered;


    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) = 0;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) = 0;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) = 0;
signals:

};

#endif // GAMEMAPOBJECT_H
