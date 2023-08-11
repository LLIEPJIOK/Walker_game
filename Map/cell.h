
#ifndef CELL_H
#define CELL_H
#include "gamemapobject.h"

class Cell : public QObject, public GameMapObject
{
    Q_OBJECT
public:
    explicit Cell(QObject* parent = nullptr, int width = 0, int height = 0, QBrush brush = QBrush(Qt::NoBrush), QString tile_name = "forest");
    void highlight();
    void normalize();
private:
    QPixmap tile;
    bool is_way;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
    void cell_signal();
    void way_to_go();
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

#endif // CELL_H
