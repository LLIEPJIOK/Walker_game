
#ifndef INVENTORY_MODIFIED_H
#define INVENTORY_MODIFIED_H


#include "Inventory/item.h"
#include <QWidget>
#include<QListWidget>


class Inventory_modified : public QListWidget
{
    Q_OBJECT
private:
    QPixmap background;
public:
    Inventory_modified();
    Inventory_modified(QWidget* parent);
protected:
    void mousePressEvent(QMouseEvent* event);
public slots:
    void hovered(QListWidgetItem *item);
signals:
    void entered(Item* item);
    void equip_change(Item* item, bool primary);
    void potion_used(Item* item);
};

#endif // INVENTORY_MODIFIED_H
