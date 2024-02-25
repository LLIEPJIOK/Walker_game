#ifndef DROPITEM_H
#define DROPITEM_H

class DropItem : public QFrame
{
    Q_OBJECT
public:
    DropItem(QString _item_type, QString _place, QString standart_item, QWidget* parent = 0);

    // меняет картинку на данную, смотрит, стандартная ли она
    void change_icon(QPixmap pix, bool _is_standart = false);

    // возвращает экипирован ли слот
    bool is_equiped();
public slots:
    void switch_equip(QString eq_name);

protected:
    void mousePressEvent(QMouseEvent* event);
    virtual void paintEvent(QPaintEvent *event);

private:
    bool is_standart;
    QString item_type;
    QString place;
    QPixmap standart;
    QPixmap current;
};

#endif // DROPITEM_H
