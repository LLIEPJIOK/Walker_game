
#include "inventory_modified.h"
#include "Inventory/item.h"
#include<QMenu>
#include<QListWidgetItem>
#include<QSize>

Inventory_modified::Inventory_modified()
{

}

Inventory_modified::Inventory_modified(QWidget *parent) : QListWidget(parent){
    this->setStyleSheet("QListWidget{background-image : url(:/backgrounds/Pictures/widget_backgrounds/status.png);"
                        "border : 0px;}"
                        "QListWidget::item {}");

    this->setMouseTracking(true);
    connect(this, &QListWidget::itemEntered, this, &Inventory_modified::hovered);
}

void Inventory_modified::mousePressEvent(QMouseEvent *event)
{
    QListWidgetItem* item = this->itemAt(event->pos());

    if (!item)
        return;

    if (dynamic_cast<Item*>(item)->get_connected_item()->get_class() == "potion"){
        if(event->button() == Qt::RightButton){
            emit potion_used(dynamic_cast<Item*>(item));
        }

        event->accept();
        return;
    }

    emit equip_change(dynamic_cast<Item*>(item), event->button() == Qt::LeftButton);
    event->accept();
}

void Inventory_modified::hovered(QListWidgetItem *item)
{
    emit entered(dynamic_cast<Item*>(item));
}
