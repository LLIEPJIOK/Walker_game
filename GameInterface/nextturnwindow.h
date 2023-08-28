#ifndef NEXTTURNWINDOW_H
#define NEXTTURNWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

class NextTurnWindow : public QWidget
{
    Q_OBJECT
public:
    explicit NextTurnWindow(QWidget *parent = nullptr, QGraphicsScene* scene = nullptr);

signals:

};

#endif // NEXTTURNWINDOW_H
