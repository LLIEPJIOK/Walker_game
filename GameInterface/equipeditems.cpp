#include "equipeditems.h"

#define eqp(item) equip(inventory->get_items().at(inventory->find_equip(item)), false)

EquipedItems::EquipedItems(Player* _player, Inventory *_inventory, QWidget *parent) :
    QFrame(parent)
{
    player = _player;
    inventory = _inventory;

    setFixedSize(0.39 * parent->width(), 0.556 * parent->height());
    setFrameStyle(QFrame::Panel | QFrame::Plain);
    setAutoFillBackground(true);

    main_hand = new DropItem("одноручное", "основная_рука", "sword", this);
    main_hand->setGeometry(0.06 * width(), 0.075 * height(), 0.19 * width(), 0.4375 * height());

    non_dominant_hand = new DropItem("одноручное", "неосновная_рука", "shield", this);
    non_dominant_hand->setGeometry(0.74 * width(), 0.075 * height(), 0.19 * width(), 0.4375 * height());

    first_ring = new DropItem("кольцо", "первое_кольцо", "first_ring", this);
    first_ring->setGeometry(0.27 * width(), 0.3625 * height(), 0.12 * width(), 0.15 * height());

    second_ring = new DropItem("кольцо", "второе_кольцо", "second_ring", this);
    second_ring->setGeometry(0.61 * width(), 0.3625 * height(), 0.12 * width(), 0.15 * height());

    necklace = new DropItem("ожерелье", "ожерелье", "necklace", this);
    necklace->setGeometry(0.61 * width(), 0.1875 * height(), 0.12 * width(), 0.15 * height());

    helmet = new DropItem("шлем", "шлем", "helmet", this);
    helmet->setGeometry(0.41 * width(), 0.025 * height(), 0.18 * width(), 0.225 * height());

    plate = new DropItem("нагрудник", "нагрудник", "plate", this);
    plate->setGeometry(0.41 * width(), 0.275 * height(), 0.18 * width(), 0.3375 * height());

    belt = new DropItem("пояс", "пояс", "belt", this);
    belt->setGeometry(0.41 * width(), 0.6375 * height(), 0.18 * width(), 0.0625 * height());

    leggings = new DropItem("поножи", "поножи", "pants", this);
    leggings->setGeometry(0.41 * width(), 0.725 * height(), 0.18 * width(), 0.25 * height());

    gloves = new DropItem("перчатки", "перчатки", "gloves", this);
    gloves->setGeometry(0.21 * width(), 0.5375 * height(), 0.18 * width(), 0.225 * height());

    boots = new DropItem("сапоги", "сапоги", "boots", this);
    boots->setGeometry(0.61 * width(), 0.5375 * height(), 0.18 * width(), 0.225 * height());
}

// обновляет экипированные предметы
void EquipedItems::update_equiped()
{
    update_equiped_container(player->get_equiped_armourment());
    update_equiped_container(player->get_equiped_weaponary());
    update_equiped_container(player->get_equiped_jewellery());
}

// нахождит нужное места для эквипа и туда его ставит
void EquipedItems::find_place(Equipment *equip)
{
    auto type = QString::fromStdString(equip->get_type());
    if (type == "одноручное")
    {
        if (!main_hand->is_equiped())
        {
            // см. дефайн
            main_hand->eqp(equip);
        }
        else
        {
            // см. дефайн
            non_dominant_hand->eqp(equip);
        }
    }
    else if (type == "кольцо")
    {
        if (!first_ring->is_equiped())
        {
            // см. дефайн
            first_ring->eqp(equip);
        }
        else
        {
            // см. дефайн
            second_ring->eqp(equip);
        }
    }
    else if (type == "ожерелье")
    {
        // см. дефайн
        necklace->eqp(equip);
    }
    else if (type == "шлем")
    {
        // см. дефайн
        helmet->eqp(equip);
    }
    else if (type == "нагрудник")
    {
        // см. дефайн
        plate->eqp(equip);
    }
    else if (type == "пояс")
    {
        // см. дефайн
        belt->eqp(equip);
    }
    else if (type == "поножи")
        {
        leggings->eqp(equip);
    }
    else if (type == "перчатки")
    {
        // см. дефайн
        gloves->eqp(equip);
    }
    else
    {
        // см. дефайн
        boots->eqp(equip);
    }
}

void EquipedItems::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPixmap invetory_pic("../Game/Resources/Pictures/inventory.png");
    invetory_pic = invetory_pic.scaled(width(), height());

    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0, 0, invetory_pic);
    painter.end();
}

// обновляет конкретный контейнер экиперованных предметов
// обновляет конкретный контейнер экиперованных предметов
template<typename T>
void EquipedItems::update_equiped_container(std::map<std::string, T> *equipment)
{
    foreach (auto equip, *equipment)
    {
        if (equip.second != nullptr && !equip.second->get_front_equiped())
        {
            equip.second->change_front_equiped();
            find_place(equip.second);
        }
    }
}
