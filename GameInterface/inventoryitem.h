
#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H


#include <QLabel>
#include "Engine/Equipment.h"
#include "Engine/Turn.h"
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
    void switchText();
    void switch_equiped();
    Equipment* get_connected_item();
protected:
    QLabel* item_name;
    bool is_equiped;
    Equipment* connected_item;
    bool is_entered;
    virtual void paintEvent(QPaintEvent *event);
private:
    void enterEvent(QEnterEvent *event);
    void leaveEvent(QEvent *event);


signals:
    //void item_right_clicked(Equipment* item);
    //void item_left_clicked(Equipment* item);
};



class DragItem : public InventoryItem
{
    Q_OBJECT
public:
    explicit DragItem( QWidget* parent = 0, Equipment* item = nullptr, QString icon = "");
    void set_equiped();
    QPixmap get_icon();
protected:
    void mousePressEvent( QMouseEvent* event );
    void mouseMoveEvent( QMouseEvent* event );
private:
    QPixmap icon;
    QPoint m_dragStart;
signals:
    void item_was_equiped(Equipment* item, QString place);
    void item_was_unequiped(Equipment* item, QString place);
public slots:
    void equip(QString place);
    void unequip(QString place);
};



class DropItem : public InventoryItem
{
    Q_OBJECT
public:
    explicit DropItem( QWidget* parent = 0, Equipment* item = nullptr, QString item_type = "", QString place = "", QString standart_item = "");
    void change_icon(QPixmap pix);
protected:
    void mousePressEvent(QMouseEvent* event);
    void dragEnterEvent( QDragEnterEvent* event );
    void dropEvent( QDropEvent* event );
    virtual void paintEvent(QPaintEvent *event);
private:
    bool is_standart;
    QString item_type;
    QString place;
    DragItem* equiped_item;
    QPixmap standart;
    QPixmap current;
};
#endif // INVENTORYITEM_H
