
#ifndef MOVINGAREA_H
#define MOVINGAREA_H


#include "HexMap.h"

class MovingArea : public QFrame
{
    Q_OBJECT
public:
    explicit MovingArea(QWidget *parent = nullptr);
private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint old_pos;
signals:

};

#endif // MOVINGAREA_H
