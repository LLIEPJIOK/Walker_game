
#ifndef EQUIPEDITEMS_H
#define EQUIPEDITEMS_H
#include "inventoryitem.h"


class EquipedItems : public QFrame
{
    Q_OBJECT
public:
    explicit EquipedItems(QWidget * parent = nullptr);
private:
    DropItem* main_hand;
    DropItem* non_dominant_hand;

    DropItem* helmet;
    DropItem* plate;
    DropItem* leggings;
    DropItem* gloves;
    DropItem* boots;

    DropItem* first_ring;
    DropItem* second_ring;
    DropItem* necklace;
    DropItem* bracelet;
    DropItem* belt;
protected:
    void paintEvent(QPaintEvent* event);
};

#endif // EQUIPEDITEMS_H
