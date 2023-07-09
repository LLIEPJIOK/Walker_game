
#ifndef MOVINGAREA_H
#define MOVINGAREA_H


#include <QWidget>
#include <QFrame>
#include <QMouseEvent>
#include "gamemap.h"
class MovingArea : public QFrame
{
    Q_OBJECT
public:
    explicit MovingArea(QWidget *parent = nullptr, GameMap* battle_map = nullptr);
private:
    GameMap *battle_map;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint old_pos;
signals:

};

#endif // MOVINGAREA_H
