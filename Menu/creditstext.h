#ifndef CREDITSTEXT_H
#define CREDITSTEXT_H

#include <QWidget>
#include <QObject>
#include <QApplication>
#include <QScreen>
#include <QPainter>
#include <QGraphicsItem>
#include <QTimer>

class CreditsText : public QGraphicsItem
{
public:
    CreditsText();
    void reset();
private:
    QPainter *painter;
    QSize size;
    QTimer *timer;
protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // CREDITSTEXT_H
