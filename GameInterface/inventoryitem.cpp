
#include "inventoryitem.h"
InventoryItem::InventoryItem(QWidget* parent, Equipment* item) :
    QFrame(parent)
{
    is_entered = false;
    is_equiped = false;
    item_name = new QLabel(this);
    if(item)
    {
        connected_item = item;
        item_name->setText(QString::fromStdString(item->get_name() + "(" + connected_item->get_type() + ")"));
    }
    item_name->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item_name->setGeometry(5, 0, parent->width() - 5, 40);
    item_name->setFont(QFont("Arial", 20));
    setFrameStyle(QFrame::Panel |QFrame::Plain);
    setLineWidth(0);

}

void InventoryItem::switchText()
{
    item_name->setText(QString::fromStdString(connected_item->get_name() + "(" + connected_item->get_type() + ")"));
}

void InventoryItem::switch_equiped()
{
    is_equiped = false;
}

Equipment *InventoryItem::get_connected_item()
{
    return connected_item;
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
        setLineWidth(0);
}


DragItem::DragItem(QWidget *parent, Equipment *item, QString icon) :
    InventoryItem(parent, item)
{
    this->icon.load("../Game/Resources/Pictures/Equipment/"+icon+".png");
}

void DragItem::set_equiped()
{
    item_name->setText(QString::fromStdString(connected_item->get_name()) + " (экипировано)");
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
    if(event->button() == Qt::RightButton && connected_item->get_class() == "зелье")
    {
        Turn::get_Turn()->get_player()->use_potion(dynamic_cast<Potion*>(connected_item));
        delete this;
    }
}

void DragItem::mouseMoveEvent(QMouseEvent *event)
{
    if(( event->buttons() & Qt::LeftButton ) &&
        QApplication::startDragDistance() <= ( event->pos() - m_dragStart ).manhattanLength() &&
        connected_item->get_class() != "зелье" &&
        !is_equiped)
    {
        QDrag* drag = new QDrag( this );
        QMimeData* mimeData = new QMimeData;

        QString type = QString::fromStdString(connected_item->get_type());
        QByteArray data = type.toUtf8();

        mimeData->setData("application/" + type, data);
        mimeData->setText(QString::fromStdString(connected_item->get_name()));
        drag->setMimeData(mimeData);

        Qt::DropAction result = drag->exec( Qt::MoveAction );
        qDebug() << "Drop action result: " << result;
        if( result == Qt::MoveAction ) {
            set_equiped();
        }
    }
}


DropItem::DropItem(QWidget *parent, Equipment* item, QString item_type, QString place, QString standart_item) :
    InventoryItem(parent, item)
{
    this->place = place;
    this->item_type = item_type;
    equiped_item = nullptr;
    standart.load("../Game/Resources/Pictures/Equipment/"+standart_item+".png");
    current = standart;
    is_standart = true;
    setAcceptDrops( true );
}

void DropItem::change_icon(QPixmap pix)
{
    current = pix;
    is_standart = !is_standart;
    update();
}

void DropItem::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        if(equiped_item)
        {
            equiped_item->unequip(place);
            equiped_item = nullptr;
            change_icon(standart);
        }
    }
}

void DropItem::dragEnterEvent(QDragEnterEvent *event)
{
    QStringList formats = event->mimeData()->formats();
    qDebug() << formats;
    if(formats.contains("application/" + item_type)) {
        event->acceptProposedAction();
    }
}

void DropItem::dropEvent(QDropEvent *event)
{
    if(equiped_item)
        equiped_item->unequip(place);
    equiped_item = qobject_cast<DragItem*>(event->source());
    equiped_item->equip(place);
    equiped_item->set_equiped();
    change_icon(equiped_item->get_icon());
    event->acceptProposedAction();
}

void DropItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(is_standart)
        painter.setOpacity(0.4);
    current = current.scaled(width(), height());
    painter.drawPixmap(0, 0, current);
    QFrame::paintEvent(event);
    if(is_entered)
    {
        setLineWidth(2);
    }
    else
        setLineWidth(1);
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
        is_equiped = false;
        emit item_was_unequiped(connected_item, place);
        switchText();
    }
}


