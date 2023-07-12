#ifndef DRAGITEM_H
#define DRAGITEM_H

#include "inventoryitem.h"

class DragItem : public InventoryItem
{
    Q_OBJECT

public:
    explicit DragItem(Equipment* item, QString icon, QWidget* parent = 0);
    void set_equiped();
    QPixmap get_icon();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    QPixmap icon;
    QPoint m_dragStart;

signals:
    void item_was_equiped(Equipment* item, QString place);
    void item_was_unequiped(Equipment* item, QString place);

public slots:
    void equip(QString place);
    void unequip(QString place);
};

#endif // DRAGITEM_H
