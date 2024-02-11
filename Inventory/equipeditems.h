#ifndef EQUIPEDITEMS_H
#define EQUIPEDITEMS_H

#include "Engine/Player.h"
#include "inventory.h"
#include "dropitem.h"

class EquipedItems : public QFrame
{
    Q_OBJECT

public:
    explicit EquipedItems(Player* _player, Inventory* _inventory, QWidget * parent = nullptr);

    // обновляет экипированные предметы
    void update_equiped();

private:

    // игрок, которому принадлежат слоты для экипировки
    Player* player;

    // инвентарь игрока
    Inventory *inventory;

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

    // обновляет конкретный контейнер экиперованных предметов
    template<typename T>
    void update_equiped_container(std::map<std::string, T>* equipment);

    // нахождит нужное места для эквипа и туда его ставит
    void find_place(Equipment* equip);

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // EQUIPEDITEMS_H
