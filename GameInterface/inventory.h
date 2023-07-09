
#ifndef INVENTORY_H
#define INVENTORY_H


#include <QWidget>
#include "inventoryitem.h"
#include <QVBoxLayout>
class Inventory : public QWidget
{
    Q_OBJECT
public:
    explicit Inventory(QWidget *parent = nullptr);
    ~Inventory();
private:
    std::vector<InventoryItem*> items;
    QVBoxLayout *list;
signals:
    void item_was_equiped(Equipment* item, QString place);
    void item_was_unequiped(Equipment* item, QString place);
protected:
    void paintEvent(QPaintEvent* event);
public slots:
    void add_new_item(Equipment* item);
};

#endif // INVENTORY_H
