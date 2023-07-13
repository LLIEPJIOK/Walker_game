#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include "gamemap.h"
#include "minimap.h"
#include "inventory.h"
#include "Menu/menu.h"
#include "equipeditems.h"
#include "pausemenu.h"
#include "Engine/DataBase.h"
#include "Engine/Turn.h"
#include "actionwindow.h"

#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>


class GameInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameInterface(QWidget *parent = nullptr);
    ~GameInterface();

public slots:
    // обновляет инвентарь, слоты экипировки, лэйблы текщуго игрока
    void update_player_status();

    // создание окна события при вызове события
    void process_event_start();

    // обработка подбора предмета
    void process_item_pick();

private:

    bool is_load;

    QSize screen_size;

    Menu* menu;

    QPushButton *menu_button;
    QPushButton *roll_button;
    QPushButton *next_turn_button;
    QPushButton *inventory_button;

    ActionWindow *action;

    Turn*  turn;
    DataBase* data_base;

    QVector<Inventory*> inventories;
    Inventory *current_inventory;

    PauseMenu* pause;

    QVector<EquipedItems*> equipment_slots;
    EquipedItems* current_equipment_slot;


    GameMap *current_map;
    MiniMap *mini_map;

    QLabel *players_name;
    QLabel *players_attack;
    QLabel *players_roll;
    QLabel *players_health_points;
    QLabel *players_armour;

    void initialize();
    void end_game();

    // восстановление состояния кнопок
    void update_buttons();

    // обновляет конкретный инвентарь и слоты для экипировки по номеру
    void update_inventory_and_slots(int id);

    // обновляет все инвентари и слоты для экипировки
    void update_all_inventories_and_slots();

    // обновляет все лейблы
    void update_labels();

    // обновляет карту
    void update_map();

    // обновляет всё вышеперечисленное
    void update_all();

protected:
    void paintEvent(QPaintEvent* event);

private slots:
    void start(std::vector<std::pair<std::string, std::string>> data);
    void load();
    void inventory_button_clicked();
    void next_turn_button_clicked();
    void roll_button_clicked();
    void enable_next_button();
    void add_item(Equipment* item);
    void pause_button();
    void remaining_rolls();
    void continue_playing();
    void to_main();
    void process_equip(Equipment* item, QString place);
    void process_unequip(Equipment* item, QString place);

    void congratulate_the_winner();
};

#endif // GAME_INTERFACE_H
