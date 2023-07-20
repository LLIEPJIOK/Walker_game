#ifndef CREDITSTEXT_H
#define CREDITSTEXT_H

#include <QWidget>
#include <QObject>
#include <QApplication>
#include <QScreen>
#include <QPainter>
#include <QGraphicsItem>
#include <QTimer>

class CreditsText : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    CreditsText(QObject *parent = nullptr);
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

#endif // CREDITSTEXT_H
