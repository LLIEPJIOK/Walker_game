
#ifndef ITEM_H
#define ITEM_H

#include "Engine/Equipment.h"
#include<QListWidgetItem>


class Item : public QListWidgetItem
{
private:
    Equipment* connected_item;

    QPixmap item_info;
public:
    Item();
    Item(Equipment* eq);

    bool being_painted;

    void update_info();
    Equipment* get_connected_item();
protected:
};

#endif // ITEM_H
