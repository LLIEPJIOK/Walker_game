

#include "inventory.h"

Inventory::Inventory(QWidget *parent)
    : QWidget{parent}
{
    setGeometry(20, 20, 0.3125 * parent->width(), 0.91*parent->height());
    list = new QVBoxLayout(this);
    list->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setLayout(list);
}

Inventory::~Inventory()
{
    for(auto& i : items)
        delete i;
}

void Inventory::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPixmap inventory("../Game/Resources/Pictures/inventory.png");
    inventory = inventory.scaled(this->width(), this->height());
    QPainter painter(this);
    painter.drawPixmap(0, 0, inventory);

}

void Inventory::add_new_item(Equipment *item)
{
    QString icon;
    std::string t = item->get_type();
    if(t == "одноручное"){
        icon = "sword";
    }
    else if(t == "кольцо")
        icon = "first_ring";
    else if(t == "шлем")
        icon = "helmet";
    else if(t == "нагрудник")
        icon = "plate";
    else if(t == "пояс")
        icon = "belt";
    else if(t == "поножи")
        icon = "pants";
    else if(t=="сапоги")
        icon = "boots";
    else if (t == "перчатки")
        icon = "gloves";
    else if(t == "ожерелье")
        icon = "necklace";
    DragItem * inventory_item = new DragItem(this, item, icon);
    inventory_item->setFixedSize(width() - 30, 40);
    inventory_item->setVisible(true);
    list->addWidget(inventory_item, Qt::AlignTop);
    connect(inventory_item, &DragItem::item_was_equiped, this, &Inventory::item_was_equiped);
    connect(inventory_item, &DragItem::item_was_unequiped, this, &Inventory::item_was_unequiped);
    items.push_back(inventory_item);
}

