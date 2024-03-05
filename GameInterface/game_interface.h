#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include "GameInterface/general_info_widget.h"
#include "informationwindow.h"
#include "Menu/menu.h"
#include "Inventory/equipeditems.h"
#include "pausemenu.h"
#include "Engine/DataBase.h"
#include "Engine/Turn.h"
#include "Engine/saveandloadmanager.h"
#include "GameInterface/player_status_widget.h"
#include "Map/minimap.h"


#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>
#include <QPlainTextEdit>

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
    QFont font;
    QString style;


    bool game_is_played;
    bool game_is_paused;
    bool is_load;
    QSize screen_size;

    Turn*  turn;
    DataBase* data_base;
    SaveAndLoadManager* save_load_manager;

    Menu* menu;
    InformationWindow* information_window;
    QPlainTextEdit *action;
    PauseMenu* pause;

    HexMap* hex_map;
    MiniMap* mini_map;

    QVector<QPushButton*> buttons;

    General_info_widget *current_info_widget;

//    QVector<Inventory*> inventories;
//    Inventory *current_inventory;

//    QVector<EquipedItems*> equipment_slots;
//    EquipedItems* current_equipment_slot;

//    QVector<Player_status_widget*> players_statuses;
//    Player_status_widget* current_player_status;

    QMap<int, void(GameInterface::*)()> key_to_action;

    void initialize();
    void create_buttons();
    void show_player();
    void end_game();

    // восстановление состояния кнопок
    void update_buttons();

    // обновляет конкретный инвентарь и слоты для экипировки по номеру
    void update_info_widget();
\
    // обновляет карту
    void update_map();

    // обновляет всё вышеперечисленное
    void update_all();

    void send_map_data();
protected:
    void paintEvent(QPaintEvent* event);
    void keyPressEvent(QKeyEvent* event);

private slots:
    void start(std::vector<std::pair<std::string, std::string>> data);
    void load(QString file_name);

    void inventory_button_clicked();
    void next_turn_button_clicked();
    void roll_button_clicked();
    void info_button_clicked();

    void enable_next_button();
    void add_item(Equipment* item);
    void pause_button();
    void to_main();

    void congratulate_the_winner();

    void save_game(QString file_name);

    void all_is_ready();

    // Multiplayer
    void set_cell_data(game_msg msg);
    void add_item_m(game_msg msg);
    void apply_effect(game_msg msg);
    void next_turn(game_msg msg);
    void unequip_place(game_msg msg);
    void equip_item(game_msg msg);
    void use_potion(game_msg msg);
    void process_attack(game_msg msg);
};

#endif // GAME_INTERFACE_H
