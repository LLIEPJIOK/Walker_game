#ifndef EQUIPEDITEMS_H
#define EQUIPEDITEMS_H

#include "Engine/Player.h"
#include "dropitem.h"

class EquipedItems : public QFrame
{
    Q_OBJECT

public:
    explicit EquipedItems(QWidget * parent = nullptr);

    // обновляет экипированные предметы
    void update_equiped();
    void set_connected_place_item(QString place, Item* item);
public slots:
    void place(Equipment* equip, QString place);
    void unequip(QString place);

private:
    QMap<QString, DropItem*> place_to_name;
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // EQUIPEDITEMS_H
