#ifndef CREDITSTEXT_H
#define CREDITSTEXT_H



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
