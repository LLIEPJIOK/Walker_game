
#include "item.h"

Item::Item()
{
    this->setText("NOT HOVERED");
    this->connected_item = new Equipment(100, "Rusty sword", "class", {{"ATK", 20}, {"CRIT_CH", 20}, {"CRIT_DMG", 20}}, "one handed");
}

Item::Item(Equipment *eq)
{
    connected_item = eq;
    this->setText(eq->get_name().c_str());
}

void Item::update_info()
{

}

Equipment *Item::get_connected_item()
{
    return connected_item;
}
