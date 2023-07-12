#ifndef INVENTORY_H
#define INVENTORY_H

#include "dragitem.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QVector>
#include <Engine/Player.h>

class Inventory : public QWidget
{
    Q_OBJECT

public:
    explicit Inventory(Player* _player, QWidget *parent = nullptr);
    ~Inventory();

    // обновление инвентаря
    void update_inventory();

    // возвращает игрока, которому принадлежит инвентарь
    Player* get_player() const;

    // находит индекс предмета,
    // если его нет возвращает -1
    int find_equip(Equipment* equip) const;

    // возвращает константную ссылку на вектор предметов
    const QVector <DragItem*>& get_items() const;

private:
    QVector <DragItem*> items;
    Player* player;
    QVBoxLayout *list;

    // обновляет весь инвентарь
    void update_all();

    // обновление экипированных предметов
    void update_equiped();

    // обновляет конкретный контейнер предметов игрока
    template <typename T>
    void update_container(std::multiset <T, Equipment_Comparator>* equipment);

    // обновляет конкретный контейнер экиперованных предметов игрока
    template <typename T>
    void update_equiped_container(std::map<std::string, T>* equipment);

signals:
    void item_was_equiped(Equipment* item, QString place);
    void item_was_unequiped(Equipment* item, QString place);

protected:
    void paintEvent(QPaintEvent* event);

public slots:
    void add_new_item(Equipment* item);
};

#endif // INVENTORY_H
