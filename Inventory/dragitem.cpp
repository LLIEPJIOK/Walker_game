#include "dragitem.h"
#include "Engine/Turn.h"
#include "Engine/translator.h"

DragItem::DragItem(Equipment *item, QString icon, QWidget *parent) :
    InventoryItem(parent, item)
{
    this->icon.load(":/equipment/Pictures/Equipment/"+icon+".png");
}

void DragItem::set_equiped()
{
    item_name->setText(QString::fromStdString(Translator::translate(connected_item->get_name().c_str())) + " (" + tr("equipped") + ")");
    is_equiped = true;
}

QPixmap DragItem::get_icon()
{
    return icon;
}

void DragItem::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        m_dragStart = event->pos();
    if(event->button() == Qt::RightButton && connected_item->get_class() == "potion")
    {
        Turn::get_Turn()->get_player()->use_potion(dynamic_cast<Potion*>(connected_item));
        emit potion_was_used();
        delete this;
    }
}

void DragItem::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) &&
        QApplication::startDragDistance() <= ( event->pos() - m_dragStart ).manhattanLength() &&
        connected_item->get_class() != "potion" &&
        !is_equiped)
    {
        QDrag* drag = new QDrag(this);
        QMimeData* mimeData = new QMimeData;

        QString type = QString::fromStdString(connected_item->get_type());
        QByteArray data = type.toUtf8();

        mimeData->setData("application/" + type, data);
        mimeData->setText(QString::fromStdString(Translator::translate(connected_item->get_name().c_str())));
        drag->setMimeData(mimeData);

        Qt::DropAction result = drag->exec(Qt::MoveAction);
        qDebug() << "Drop action result: " << result;

    }
}

void DragItem::equip(QString place)
{
    unequip(place);
    emit item_was_equiped(connected_item, place);
}

void DragItem::unequip(QString place)
{
    if(is_equiped)
    {
        connected_item->change_equiped();
        connected_item->change_front_equiped();
        is_equiped = false;
        emit item_was_unequiped(connected_item, place);
        switch_text();
    }
}
