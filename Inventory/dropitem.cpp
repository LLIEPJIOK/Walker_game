#include "dropitem.h"

DropItem::DropItem(QString _item_type, QString _place, QString standart_item, QWidget *parent) :
    InventoryItem(parent)
{
    item_type = _item_type;
    place = _place;
    equiped_item = nullptr;
    standart.load("../Game/Resources/Pictures/Equipment/" + standart_item + ".png");
    current = standart;
    is_standart = true;
    setAcceptDrops(true);
}

// меняет картинку на данную, смотрит, стандартная ли она
void DropItem::change_icon(QPixmap pix, bool _is_standart)
{
    current = pix;
    is_standart = _is_standart;
    update();
}

void DropItem::equip(DragItem *item, bool connect_back)
{
    if (equiped_item != nullptr)
    {
        equiped_item->unequip(place);
    }

    equiped_item = item;
    if (connect_back)
    {
        equiped_item->equip(place);
    }

    equiped_item->get_connected_item()->change_front_equiped();
    equiped_item->set_equiped();
    change_icon(equiped_item->get_icon());
}

bool DropItem::is_equiped()
{
    return equiped_item != nullptr;
}

void DropItem::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        if (equiped_item)
        {
            equiped_item->unequip(place);
            equiped_item = nullptr;
            change_icon(standart, 1);
        }
    }
}

void DropItem::dragEnterEvent(QDragEnterEvent *event)
{
    QStringList formats = event->mimeData()->formats();
    if(formats.contains("application/" + item_type))
    {
        event->acceptProposedAction();
    }
}

void DropItem::dropEvent(QDropEvent *event)
{
    equip(qobject_cast<DragItem*>(event->source()));
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
    InventoryItem::paintEvent(event);
    if(is_entered)
    {
        setLineWidth(2);
    }
    else
        setLineWidth(1);
}
