#include "general_info_widget.h"
#include "Inventory/item.h"

#include<QPushButton>
#include<QPainter>

General_info_widget::General_info_widget(QWidget *parent, Player *pl) : QWidget(parent)
{
    assigned_player = pl;
    background = QPixmap(":/backgrounds/Pictures/widget_backgrounds/status.png");
    background = background.scaled(width(), height());

    this->setGeometry(0, 0, QApplication::screens().at(0)->size().width(), QApplication::screens().at(0)->size().height());

    sections = new QTabWidget(this);
    sections->setGeometry(0, 0, 0.25 * width(), height());
    sections->setStyleSheet("QTabWidget::pane {"
                            "border: 1px solid black;"
                            "top:-1px; "
                            "background-image: url(:/backgrounds/Pictures/widget_backgrounds/status.png);"
                            "}"
                            "QTabBar::tab {"
                            "border: 1px solid lightblack;"
                            "height : 55;"
                            "width : 80;"
                            " background-image: url(:/backgrounds/Pictures/widget_backgrounds/status.png)"
                            "} "
                            "QTabBar::tab:selected { "
                            "border: 2px solid black; "
                            "margin-bottom: -1px; "
                            "background-image: url(:/backgrounds/Pictures/widget_backgrounds/status.png)"
                            "}");

    status = new Player_status_widget(this, pl);
    status->setGeometry(20, 20, 0.25 * width(), 520);
    sections->addTab(status, tr("Status"));

    class_to_inventory["weapon"] = new Inventory_modified(this);
    class_to_inventory["weapon"]->setGeometry(20, 540, 0.25 * width(), 0.45 * height());
    sections->addTab(class_to_inventory["weapon"], tr("Weapons"));

    class_to_inventory["armour"] = new Inventory_modified(this);
    class_to_inventory["armour"]->setGeometry(20, 540, 0.25 * width(), 0.45 * height());
    sections->addTab(class_to_inventory["armour"], tr("Armour"));

    class_to_inventory["jewel"] = new Inventory_modified(this);
    class_to_inventory["jewel"]->setGeometry(20, 540, 0.25 * width(), 0.45 * height());
    sections->addTab(class_to_inventory["jewel"], tr("Jewels"));
    class_to_inventory["jewel"]->addItem(new Item(new Jewel("belt", 16)));
    class_to_inventory["jewel"]->addItem(new Item(new Jewel("ring", 16)));
    class_to_inventory["jewel"]->addItem(new Item(new Jewel("necklace", 16)));

    class_to_inventory["potion"] = new Inventory_modified(this);
    class_to_inventory["potion"]->setGeometry(20, 540, 0.25 * width(), 0.45 * height());
    sections->addTab(class_to_inventory["potion"], tr("Potions"));


    equipped_items = new EquipedItems(this);
    equipped_items->setGeometry(0.25 * parent->width(), 20, parent->width() - 370, parent->height());
    equipped_items->update_equiped();

    image = new Image_scene(this);
    image->setGeometry(equipped_items->x() + equipped_items->width(), 20, parent->width() - (equipped_items->x() + equipped_items->width()), height() - 40);

    QPushButton* back = new QPushButton("BACK", this);
    back->setGeometry(width() - 200, height() - 50, 160, 40);
    connect(back, &QPushButton::clicked, this, &General_info_widget::back);

    connect(class_to_inventory["jewel"], &Inventory_modified::entered, image, &Image_scene::take_item);
    connect(class_to_inventory["weapon"], &Inventory_modified::entered, image, &Image_scene::take_item);
    connect(class_to_inventory["armour"], &Inventory_modified::entered, image, &Image_scene::take_item);
    connect(class_to_inventory["potion"], &Inventory_modified::entered, image, &Image_scene::take_item);

    connect(class_to_inventory["jewel"], &Inventory_modified::equip_change, this, &::General_info_widget::process_equip);
    connect(class_to_inventory["weapon"], &Inventory_modified::equip_change, this, &::General_info_widget::process_equip);
    connect(class_to_inventory["armour"], &Inventory_modified::equip_change, this, &::General_info_widget::process_equip);
    connect(class_to_inventory["potion"], &Inventory_modified::potion_used, this, &General_info_widget::process_potion);
}

Player *General_info_widget::get_player()
{
    return assigned_player;
}

void General_info_widget::addItem(Equipment *item)
{
    Item* it = new Item(item);
    class_to_inventory[QString::fromStdString(item->get_class())]->addItem(it);
}

void General_info_widget::update_stats()
{
    status->update_all();
}

void General_info_widget::update_inventories()
{
    //утечка памяти ??
    for (auto& inv : class_to_inventory){
        inv->clear();
    }

    for (Equipment* eq : *assigned_player->get_items())
        addItem(eq);

    for (Potion* pot : *assigned_player->get_potions())
        addItem(pot);
}

void General_info_widget::update_equipped()
{
    for (auto& eq : *assigned_player->get_equipped_items()){
        equipped_items->unequip(QString::fromStdString(eq.first));
        equipped_items->place(eq.second, QString::fromStdString(eq.first));
    }
}

void General_info_widget::update_all()
{
    status->update_all();
    update_inventories();
    update_equipped();

}

void General_info_widget::process_equip(Item * item, bool primary)
{
    Equipment* it = item->get_connected_item();

    // if item is equipped, then we need to either swap primary and secondary, or simply unequip the item
    if (it->get_equiped()){
        if (it->get_type() == "one-handed"){
            if (primary && it->get_primary_equipped()){
                equipped_items->unequip("main_hand");
                assigned_player->unequip_item("main_hand");
                it->set_primary_equipped(false);
            }
            else if (!primary && !it->get_primary_equipped()){
                equipped_items->unequip("offhand");
                assigned_player->unequip_item("offhand");
                it->set_primary_equipped(false);
            }
            else if (primary){
                equipped_items->unequip("offhand");
                assigned_player->unequip_item("offhand");
                assigned_player->equip_item(it, "main_hand");
                equipped_items->place(it, "main_hand");
                it->set_primary_equipped(true);
            }
            else{
                equipped_items->unequip("main_hand");
                assigned_player->unequip_item("main_hand");
                assigned_player->equip_item(it, "offhand");
                equipped_items->place(it, "offhand");
                it->set_primary_equipped(false);
            }

        }
        else if (it->get_type() == "ring"){
            if (primary && it->get_primary_equipped()){
                equipped_items->unequip("first_ring");
                assigned_player->unequip_item("first_ring");
                it->set_primary_equipped(false);
            }
            else if (!primary && !it->get_primary_equipped()){
                equipped_items->unequip("second_ring");
                assigned_player->unequip_item("second_ring");
                it->set_primary_equipped(false);
            }
            else if (primary){
                equipped_items->unequip("second_ring");
                assigned_player->unequip_item("second_ring");
                assigned_player->equip_item(it, "first_ring");
                equipped_items->place(it, "first_ring");
                it->set_primary_equipped(true);
            }
            else{
                equipped_items->unequip("first_ring");
                assigned_player->unequip_item("first_ring");
                assigned_player->equip_item(it, "second_ring");
                equipped_items->place(it, "second_ring");
                it->set_primary_equipped(false);
            }
        }
        else{
            equipped_items->unequip(QString::fromStdString(it->get_type()));
            assigned_player->unequip_item(it->get_type());
        }
    }
    else
    {
        // if item wasn't equipped
        if (it->get_type() == "one-handed"){
            if (primary){
                equipped_items->unequip("main_hand");
                assigned_player->unequip_item("main_hand");
                assigned_player->equip_item(it, "main_hand");
                equipped_items->place(it, "main_hand");
                it->set_primary_equipped(true);
            }
            else{
                equipped_items->unequip("offhand");
                assigned_player->unequip_item("offhand");
                assigned_player->equip_item(it, "offhand");
                equipped_items->place(it, "offhand");
                it->set_primary_equipped(false);
            }
        }
        else if (it->get_type() == "ring"){
            if (primary){
                equipped_items->unequip("first_ring");
                assigned_player->unequip_item("first_ring");
                assigned_player->equip_item(it, "first_ring");
                equipped_items->place(it, "first_ring");
                it->set_primary_equipped(true);
            }
            else{
                equipped_items->unequip("second_ring");
                assigned_player->unequip_item("second_ring");
                assigned_player->equip_item(it, "second_ring");
                equipped_items->place(it, "second_ring");
                it->set_primary_equipped(false);
            }
        }
        else{
            equipped_items->unequip(QString::fromStdString(it->get_type()));
            equipped_items->place(it, QString::fromStdString(it->get_type()));
            assigned_player->unequip_item(it->get_type());
            assigned_player->equip_item(it, it->get_type());
        }
    }

    if (it->get_equiped())
        item->setText(QString::fromStdString(it->get_name()) + "(" + tr("equipped") + ")");
    else
        item->setText(it->get_name().c_str());


    status->update_all();
    equipped_items->update();
}

void General_info_widget::process_potion(Item *item)
{
    class_to_inventory["potion"]->removeItemWidget(item);
    class_to_inventory["potion"]->update();

    assigned_player->use_potion(dynamic_cast<Potion*>(item->get_connected_item()));
    status->update_all();
    delete item;

}

void General_info_widget::back()
{
    this->setVisible(false);
}

void General_info_widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), background);
    QWidget::paintEvent(event);
}

