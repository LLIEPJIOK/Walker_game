
#include "item.h"
#include"Engine/translator.h"

Item::Item()
{
    this->setText("NOT HOVERED");
    this->connected_item = new Equipment(100, "Rusty sword", "class", {{"ATK", 20}, {"CRIT_CH", 20}, {"CRIT_DMG", 20}}, "one handed");
}

Item::Item(Equipment *eq)
{
    connected_item = eq;
    this->setText(Translator::translate(eq->get_name().c_str()).c_str());
    this->setSizeHint(QSize(1, 50));
}

void Item::update_info()
{

}

Equipment *Item::get_connected_item()
{
    return connected_item;
}
