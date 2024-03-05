#include "equipeditems.h"
#include "Engine/translator.h"
#include<QPainter>

#define eqp(item) equip(inventory->get_items().at(inventory->find_equip(item)), false)

EquipedItems::EquipedItems(QWidget *parent) :
    QFrame(parent)
{
    setGeometry(370, 20, parent->width() - 370, parent->height());
    setFixedSize(parent->width() * 0.45 , 0.52 * parent->height() + 450);
    setFrameStyle(QFrame::Panel | QFrame::Plain);
    setAutoFillBackground(true);

    place_to_name["main_hand"] = new DropItem("one-handed", "main hand", "sword", this);
    place_to_name["main_hand"]->setGeometry(0.06 * width(), 0.075 * height(), 0.19 * width(), 0.4375 * height());

    place_to_name["offhand"] = new DropItem("one-handed", "offhand", "shield", this);
    place_to_name["offhand"]->setGeometry(0.74 * width(), 0.075 * height(), 0.19 * width(), 0.4375 * height());

    place_to_name["first_ring"] = new DropItem("ring", "first ring", "first_ring", this);
    place_to_name["first_ring"]->setGeometry(0.27 * width(), 0.3625 * height(), 0.12 * width(), 0.15 * height());

    place_to_name["second_ring"] = new DropItem("ring", "second ring", "second_ring", this);
    place_to_name["second_ring"]->setGeometry(0.61 * width(), 0.3625 * height(), 0.12 * width(), 0.15 * height());

    place_to_name["necklace"] = new DropItem("necklace", "necklace", "necklace", this);
    place_to_name["necklace"]->setGeometry(0.61 * width(), 0.1875 * height(), 0.12 * width(), 0.15 * height());

    place_to_name["helmet"] = new DropItem("helmet", "helmet", "helmet", this);
    place_to_name["helmet"]->setGeometry(0.41 * width(), 0.025 * height(), 0.18 * width(), 0.225 * height());

    place_to_name["cuirass"] = new DropItem("cuirass", "cuirass", "cuirass", this);
    place_to_name["cuirass"]->setGeometry(0.41 * width(), 0.275 * height(), 0.18 * width(), 0.3375 * height());

    place_to_name["belt"] = new DropItem("belt", "belt", "belt", this);
    place_to_name["belt"]->setGeometry(0.41 * width(), 0.6375 * height(), 0.18 * width(), 0.0625 * height());

    place_to_name["leggings"] = new DropItem("leggings", "leggings", "leggings", this);
    place_to_name["leggings"]->setGeometry(0.41 * width(), 0.725 * height(), 0.18 * width(), 0.25 * height());

    place_to_name["gloves"] = new DropItem("gloves", "gloves", "gloves", this);
    place_to_name["gloves"]->setGeometry(0.21 * width(), 0.5375 * height(), 0.18 * width(), 0.225 * height());

    place_to_name["boots"] = new DropItem("boots", "boots", "boots", this);
    place_to_name["boots"]->setGeometry(0.61 * width(), 0.5375 * height(), 0.18 * width(), 0.225 * height());
}

// обновляет экипированные предметы
void EquipedItems::update_equiped()
{

}

void EquipedItems::set_connected_place_item(QString place, Item *item)
{
    place_to_name[place]->set_current_item(item);
}

void EquipedItems::unequip(QString place)
{
    if (place_to_name[place]->get_current_item()){
        place_to_name[place]->get_current_item()->setText(Translator::translate(place_to_name[place]->get_current_item()->get_connected_item()->get_name().c_str()).c_str());
        place_to_name[place]->set_current_item(nullptr);
    }

    place_to_name[place]->change_icon(QPixmap(), true);
}

void EquipedItems::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        raise();
}

// нахождит нужное места для эквипа и туда его ставит
void EquipedItems::place(Equipment* equip, QString place)
{
    if (!equip)
        return;

    place_to_name[place]->switch_equip(QString::fromStdString(equip->get_type()));
}

void EquipedItems::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPixmap invetory_pic(":/backgrounds/Pictures/widget_backgrounds/status.png");
    invetory_pic = invetory_pic.scaled(width(), height());

    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0, 0, invetory_pic);
    painter.end();
}

