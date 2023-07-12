
#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include "gamemap.h"
#include "minimap.h"
#include "inventory.h"
#include <QPushButton>
#include <Menu/menu.h>
#include "equipeditems.h"
#include "pausemenu.h"
#include "congratulationwindow.h"
#include "Engine/saveandloadmanager.h"

class Game_Interface : public QMainWindow
{
    Q_OBJECT
public:
    explicit Game_Interface(QWidget *parent = nullptr);
    void update_characteristics();
    ~Game_Interface();
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

    QVector<EquipedItems*> wins;
    EquipedItems* current_win;


    GameMap *current_map;
    MiniMap *mini_map;

    QLabel *players_name;
    QLabel *players_attack;
    QLabel *players_roll;
    QLabel *players_health_points;
    QLabel *players_armour;

    SaveAndLoadManager* save_load_manager;

    void initialize();
    void end_game();
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
