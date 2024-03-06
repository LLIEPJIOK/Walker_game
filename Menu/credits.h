#ifndef CREDITS_H
#define CREDITS_H

#include "creditstext.h"


class Credits : public QGraphicsView
{
    Q_OBJECT
public:
    Credits(QWidget *parent = nullptr);
    void reset();
    ~Credits();
private:
    int my_timer;
    qreal y;
    QLabel *label;
    QPainter *painter;
    QSize size;
    CreditsText *tt;
    QGraphicsScene * gs;
protected:
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void open_menu_signal();
public slots:
    void sdvig();
};

#endif // CREDITS_H
