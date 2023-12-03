#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include "informationwindow.h"
#include "minimap.h"
#include "Menu/menu.h"
#include "Inventory/equipeditems.h"
#include "pausemenu.h"
#include "Engine/DataBase.h"
#include "Engine/Turn.h"
#include "actionwindow.h"
#include "Engine/saveandloadmanager.h"
#include "GameInterface/player_status_widget.h"

#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>

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
    bool game_is_played;
    bool game_is_paused;
    bool is_load;
    QSize screen_size;

    Turn*  turn;
    DataBase* data_base;
    SaveAndLoadManager* save_load_manager;

    Menu* menu;
    InformationWindow* information_window;
    ActionWindow *action;
    PauseMenu* pause;
    GameMap *current_map;
    MiniMap *mini_map;

    QVector<QPushButton*> buttons;
    QVector<QLabel*> labels;

    QVector<Inventory*> inventories;
    Inventory *current_inventory;

    QVector<EquipedItems*> equipment_slots;
    EquipedItems* current_equipment_slot;

    QVector<Player_status_widget*> players_statuses;
    Player_status_widget* current_player_status;

    QMap<int, void(GameInterface::*)()> key_to_action;
    void initialize();
    void show_player();
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
    void keyPressEvent(QKeyEvent* event);

private slots:
    void start(std::vector<std::pair<std::string, std::string>> data);
    void load(QString file_name);
    void inventory_button_clicked();
    void next_turn_button_clicked();
    void roll_button_clicked();
    void status_button_clicked();
    void enable_next_button();
    void add_item(Equipment* item);
    void pause_button();
    void remaining_rolls();
    void to_main();
    void process_equip(Equipment* item, QString place);
    void process_unequip(Equipment* item, QString place);

    void congratulate_the_winner();

    void save_game(QString file_name);

    void all_is_ready();
};

#endif // GAME_INTERFACE_H
