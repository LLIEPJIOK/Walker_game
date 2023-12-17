#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H

#include "Engine/Equipment.h"

#include <QLabel>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QPainter>
#include <QDrag>
#include <QMimeData>
#include <QApplication>
#include <QPixmap>


class InventoryItem : public QFrame
{
    Q_OBJECT

public:
    explicit InventoryItem(QWidget* parent = nullptr, Equipment* item = nullptr);
    void switch_text();
    void switch_equiped();

    // возвращает соответствующий предмет
    Equipment* get_connected_item();

    // возвращает экиперован ли предмет
    bool get_is_equiped() const;

protected:
    QLabel* item_name;
    Equipment* connected_item;
    bool is_equiped;
    bool is_entered;
    void paintEvent(QPaintEvent *event) override;

private:
    void enterEvent(QEnterEvent *event)  override;
    void leaveEvent(QEvent *event) override;

signals:
    //void item_right_clicked(Equipment* item);
    //void item_left_clicked(Equipment* item);
};

#endif // INVENTORYITEM_H
