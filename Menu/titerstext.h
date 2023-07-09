#ifndef TITERSTEXT_H
#define TITERSTEXT_H

#include <QWidget>
#include <QObject>
#include <QApplication>
#include <QScreen>
#include <QPainter>
#include <QGraphicsItem>
#include <QTimer>

class TitersText : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    TitersText(QObject *parent = nullptr);
    void reset();
private:
    QPainter *painter;
    QSize size;
    QTimer *timer;
protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private slots:

signals:
    void open_menu_signal();
};

#endif // TITERSTEXT_H
