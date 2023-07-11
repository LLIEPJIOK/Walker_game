#ifndef DROPITEM_H
#define DROPITEM_H

#include "dragitem.h"

class DropItem : public InventoryItem
{
    Q_OBJECT
public:
    explicit DropItem(QString _item_type, QString _place, QString standart_item, QWidget* parent = 0);

    // меняет картинку на данную, смотрит, стандартная ли она
    void change_icon(QPixmap pix, bool _is_standart = false);

    // экипирует предмет из инвернтаря с бэком или нет
    void equip(DragItem *item, bool connect_back = true);

    // возвращает экипирован ли слот
    bool is_equiped();

protected:
    void mousePressEvent(QMouseEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    virtual void paintEvent(QPaintEvent *event);

private:
    bool is_standart;
    QString item_type;
    QString place;
    DragItem* equiped_item;
    QPixmap standart;
    QPixmap current;
};

#endif // DROPITEM_H
