#include "dropitem.h"

DropItem::DropItem(QString _item_type, QString _place, QString standart_item, QWidget *parent) : QFrame(parent)
{
    item_type = _item_type;
    place = _place;
    standart.load(":/equipment/Pictures/Equipment/" + standart_item + ".png");
    current = standart;
    //setFrameStyle(QFrame::Panel |QFrame::Plain);
    is_standart = true;
}

// меняет картинку на данную, смотрит, стандартная ли она
void DropItem::change_icon(QPixmap pix, bool _is_standart)
{

    current = pix;
    is_standart = _is_standart;
    if (is_standart)
        current = standart;
}

bool DropItem::is_equiped()
{
    return !is_standart;
}

void DropItem::set_current_item(Item *item)
{
    current_item = item;
}

Item *DropItem::get_current_item()
{
    return current_item;
}

void DropItem::switch_equip(QString eq_name)
{
    current = QPixmap(":/equipment/Pictures/Equipment/" + eq_name + ".png");
    is_standart = false;
}

void DropItem::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {

    }
}


void DropItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (is_standart)
    {
        painter.setOpacity(0.4);
    }

    current = current.scaled(width(), height());
    painter.drawPixmap(0, 0, current);
    QFrame::paintEvent(event);
    setLineWidth(1);
}
