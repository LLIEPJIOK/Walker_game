#include "inventoryitem.h"

#include <Engine/Turn.h>

InventoryItem::InventoryItem(QWidget* parent, Equipment *item) :
    QFrame(parent)
{
    is_entered = false;
    is_equiped = false;
    item_name = new QLabel(this);
    if (item)
    {
        connected_item = item;
        item_name->setText(tr(item->get_name().c_str()) + "(" + tr(connected_item->get_type().c_str()) + ")");
    }

    item_name->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item_name->setGeometry(5, 0, parent->width() - 5, 40);
    item_name->setFont(QFont("Arial", 20));
    setFrameStyle(QFrame::Panel |QFrame::Plain);
    setLineWidth(0);
}

void InventoryItem::switch_text()
{
    item_name->setText(tr(connected_item->get_name().c_str()) + "(" + tr(connected_item->get_type().c_str()) + ")");
}

void InventoryItem::switch_equiped()
{
    is_equiped = false;
}

Equipment* InventoryItem::get_connected_item()
{
    return connected_item;
}

// возвращает экиперован ли предмет
bool InventoryItem::get_is_equiped() const
{
    return is_equiped;
}

void InventoryItem::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    is_entered = true;
    update();
}

void InventoryItem::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    is_entered = false;
    update();
}

void InventoryItem::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    if(is_entered)
    {
        setLineWidth(1);
    }
    else
    {
        setLineWidth(0);
    }
}
