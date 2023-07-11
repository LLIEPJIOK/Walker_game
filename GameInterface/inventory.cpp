#include "inventory.h"

Inventory::Inventory(Player *_player, QWidget *parent)
    : QWidget{parent}
{
    player = _player;

    setGeometry(20, 20, 0.3125 * parent->width(), 0.91 * parent->height());

    list = new QVBoxLayout();
    list->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setLayout(list);
}

Inventory::~Inventory()
{
    for(auto& i : items)
    {
        delete i;
    }
}

// обновление инвентаря
void Inventory::update_inventory()
{
    update_all();
    update_equiped();
}

Player *Inventory::get_player() const
{
    return player;
}

// находит индекс предмета,
// если его нет возвращает -1
int Inventory::find_equip(Equipment *equip) const
{
    for (int i = 0; i < (int)items.size(); ++i)
    {
        if (items[i]->get_connected_item() == equip)
        {
            return i;
        }
    }

    return -1;
}

// возвращает константную ссылку на вектор предметов
const QVector<DragItem *> &Inventory::get_items() const
{
    return items;
}

// обновляет весь инвентарь
void Inventory::update_all()
{
    update_container(player->get_armourment());
    update_container(player->get_weaponary());
    update_container(player->get_jewellery());
    update_container(player->get_potions());
}

// обновление экипированных предметов
void Inventory::update_equiped()
{
    update_equiped_container(player->get_equiped_armourment());
    update_equiped_container(player->get_equiped_weaponary());
    update_equiped_container(player->get_equiped_jewellery());
}

void Inventory::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPixmap inventory("../Game/Resources/Pictures/inventory.png");
    inventory = inventory.scaled(width(), height());

    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0, 0, inventory);
    painter.end();
}

void Inventory::add_new_item(Equipment *item)
{
    // если существует такой элемент, то выходим
    if (find_equip(item) != -1)
    {
        return;
    }

    QString icon;
    std::string type = item->get_type();

    if(type == "одноручное")
    {
        icon = "sword";
    }
    else if(type == "кольцо")
    {
        icon = "first_ring";
    }
    else if(type == "шлем")
    {
        icon = "helmet";
    }
    else if(type == "нагрудник")
    {
        icon = "plate";
    }
    else if(type == "пояс")
    {
        icon = "belt";
    }
    else if(type == "поножи")
    {
        icon = "pants";
    }
    else if(type == "сапоги")
    {
        icon = "boots";
    }
    else if (type == "перчатки")
    {
        icon = "gloves";
    }
    else if(type == "ожерелье")
    {
        icon = "necklace";
    }

    DragItem* inventory_item = new DragItem(item, icon, this);
    inventory_item->setFixedSize(width() - 30, 40);
    list->addWidget(inventory_item, Qt::AlignTop);
    items.push_back(inventory_item);

    connect(inventory_item, &DragItem::item_was_equiped, this, &Inventory::item_was_equiped);
    connect(inventory_item, &DragItem::item_was_unequiped, this, &Inventory::item_was_unequiped);
}

// обновляет конкретный контейнер предметов игрока
template<typename T>
void Inventory::update_container(std::multiset<T, Equipment_Comparator> *equipment)
{
    foreach (auto equip, *equipment)
    {
        add_new_item(equip);
    }
}

// обновляет конкретный контейнер экиперованных предметов игрока
template<typename T>
void Inventory::update_equiped_container(std::map<std::string, T> *equipment)
{
    foreach (auto equip, *equipment)
    {
        if (equip.second != nullptr && !equip.second->get_equiped())
        {
            items[find_equip(equip.second)]->set_equiped();
        }
    }
}
