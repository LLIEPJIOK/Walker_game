
#ifndef GENERAL_INFO_WIDGET_H
#define GENERAL_INFO_WIDGET_H
#include "GameInterface/image_scene.h"
#include "Inventory/inventory_modified.h"
#include"player_status_widget.h"
#include"Inventory/equipeditems.h"

#include <QWidget>


class General_info_widget : public QWidget
{
    Q_OBJECT

    Player* assigned_player;

    QTabWidget* sections;

    Player_status_widget* status;

    EquipedItems* equipped_items;
    Image_scene* image;

    QMap<QString, Inventory_modified*> class_to_inventory;

    QPixmap background;

    void initiate_inventory(Player& player);
public:
    General_info_widget(QWidget *parent, Player *pl);
    Player* get_player();
    void addItem(Equipment* item);
    void update_stats();
    void update_inventories();
    void update_equipped();
    void update_all();
public slots:
    void process_equip(Item* item, bool primary);
    void process_potion(Item* item);
    void back();
protected:
    void paintEvent(QPaintEvent *event);

};

#endif // GENERAL_INFO_WIDGET_H
