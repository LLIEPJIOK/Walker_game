
#ifndef INVENTORY_MODIFIED_H
#define INVENTORY_MODIFIED_H


#include "Inventory/item.h"


class Inventory_modified : public QListWidget
{
    Q_OBJECT
private:
    QPixmap background;

    bool state = true;
public:
    Inventory_modified();
    Inventory_modified(QWidget* parent);

    void set_playable(bool _state);
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
