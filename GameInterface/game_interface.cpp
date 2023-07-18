#include "congratulationwindow.h"
#include "game_interface.h"
#include "Menu/general.h"
#include "Events/event_window.h"

#include <QApplication>

#define get_plchar(character) turn->get_player()->get_characteristics().at(character)
#define sleep(time) std::this_thread::sleep_for(std::chrono::milliseconds(time))

GameInterface::GameInterface(QWidget *parent)
    : QMainWindow{parent}
{
    current_map = nullptr;
    mini_map = nullptr;
    is_load = false;

    screen_size = QApplication::screens().at(0)->size();

    setFixedSize(screen_size);
    showFullScreen();
    menu = new Menu();
    setCentralWidget(menu);

    //создание нового объекта класса SaveAndLoadManager
    save_load_manager = new SaveAndLoadManager("SAVE_FILE");

    connect(General::get_general(), &General::start_game, this, &GameInterface::start);
    connect(menu, &Menu::load_the_game, this, &GameInterface::load);
}

GameInterface::~GameInterface()
{
    delete turn;
    delete data_base;
    delete current_map;
    delete mini_map;
    delete menu;

    foreach (Inventory* inv, inventories)
        delete inv;

    inventories.clear();

    foreach (EquipedItems* ei, equipment_slots)
        delete ei;

    equipment_slots.clear();
}

void GameInterface::initialize()
{
    QFont font ("Arial", 14, QFont::Normal, 1);
    QString style("color: rgb(255, 255, 255)");

    menu_button = new QPushButton("| |",this);
    menu_button->setGeometry(0.8 * screen_size.width(), 10, 0.195 * screen_size.width(), 0.069 * screen_size.height());
    connect(menu_button, &QPushButton::clicked, this, &GameInterface::pause_button);
    menu_button->setFlat(1);
    menu_button->setFont(font);
    menu_button->setStyleSheet(style);
    menu_button->setVisible(true);

    roll_button = new QPushButton("Roll", this);
    roll_button->setGeometry(0.8 * screen_size.width(), 10 + 0.069 * screen_size.height(), 0.195 * screen_size.width(), 0.069 * screen_size.height());
    roll_button->setFlat(1);
    roll_button->setFont(font);
    roll_button->setStyleSheet(style);
    roll_button->setVisible(true);
    connect(roll_button, &QPushButton::clicked, this, &GameInterface::roll_button_clicked);

    next_turn_button = new QPushButton("End Turn", this);
    next_turn_button-> setGeometry(0.8 * screen_size.width(), 10 + 0.138 * screen_size.height(), 0.195 * screen_size.width(), 0.069 * screen_size.height());
    next_turn_button->setFlat(1);
    next_turn_button->setFont(font);
    next_turn_button->setStyleSheet(style);
    next_turn_button->setVisible(true);
    connect(next_turn_button, &QPushButton::clicked, this, &GameInterface::next_turn_button_clicked);

    inventory_button = new QPushButton("Inventory", this);
    inventory_button->setGeometry(0.8 * screen_size.width(), 10 + 0.207 * screen_size.height(), 0.195 * screen_size.width(), 0.069 * screen_size.height());
    inventory_button->setFlat(1);
    inventory_button->setFont(font);
    inventory_button->setStyleSheet(style);
    inventory_button->setVisible(true);
    connect(inventory_button, &QPushButton::clicked, this, &GameInterface::inventory_button_clicked);

    action = new ActionWindow(this);
    action->setGeometry(0.8 * screen_size.width(), 10 + 0.276 * screen_size.height(), 0.195 * screen_size.width(), 0.324 * screen_size.height() - 10);
    action->setVisible(true);

    for(auto& i: *data_base->get_sequence())
    {
        Inventory* inventory = new Inventory(i, this);
        inventory->setVisible(false);
        inventories.push_back(inventory);
        connect(inventory, &Inventory::item_was_equiped, this, &GameInterface::process_equip);
        connect(inventory, &Inventory::item_was_unequiped, this, &GameInterface::process_unequip);
        connect(inventory, &Inventory::potion_was_used, this, &GameInterface::update_player_status);
    }

    for(int i = 0; i < data_base->get_sequence()->size(); i++)
    {
        EquipedItems* win = new EquipedItems(data_base->get_sequence()->at(i), inventories[i], this);
        win->setVisible(false);
        win->setGeometry(0.335 * screen_size.width(), 20, 0, 0);
        equipment_slots.push_back(win);
    }

    players_name = new QLabel(this);
    players_name->setGeometry(10, 0.958 * screen_size.height(), 0.078 * screen_size.width(), 0.028 * screen_size.height());
    std::string name = turn->get_player()->get_name();
    players_name->setText("Имя: " + QString::fromStdString(name));
    players_name->setStyleSheet(style);
    players_name->setVisible(true);

    players_roll = new QLabel(this);
    players_roll->setGeometry(130, 0.958 * screen_size.height(), 0.078 * screen_size.width(), 0.028 * screen_size.height());
    players_roll->setStyleSheet(style);
    players_roll->setVisible(true);

    players_health_points = new QLabel(this);
    players_health_points->setGeometry(250, 0.958 * screen_size.height(), 0.078 * screen_size.width(), 0.028 * screen_size.height());
    players_health_points->setStyleSheet(style);
    players_health_points->setVisible(true);

    players_armour = new QLabel(this);
    players_armour->setGeometry(370, 0.958 * screen_size.height(), 0.078 * screen_size.width(), 0.028 * screen_size.height());
    players_armour->setStyleSheet(style);
    players_armour->setVisible(true);

    players_attack = new QLabel(this);
    players_attack->setGeometry(490, 0.958 * screen_size.height(), 0.078 * screen_size.width(), 0.028 * screen_size.height());
    players_attack->setStyleSheet(style);
    players_attack->setVisible(true);

    pause = new PauseMenu(this);
    pause->setVisible(false);
    connect(pause, &PauseMenu::continue_button_clicked, this, &GameInterface::continue_playing);
    connect(pause, &PauseMenu::main_menu_clicked, this, &GameInterface::to_main);

    update_all();
}

void GameInterface::end_game()
{
    delete turn;
    turn = nullptr;
    delete data_base;
    data_base = nullptr;

    foreach(Inventory* inv, inventories)
        delete inv;
    inventories.clear();
    //inventories.shrink_to_fit();

    foreach(EquipedItems* ei, equipment_slots)
        delete ei;
    equipment_slots.clear();
    //equipment_slots.shrink_to_fit();

    delete current_map;
    current_map = nullptr;
    delete mini_map;
    mini_map = nullptr;
}

void GameInterface::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(44, 66, 47)));
    painter.setPen(QPen(QColor(44, 66, 47)));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRect(0, 0 , this->width(), this->height());
    QMainWindow::paintEvent(event);
}

void GameInterface::start(std::vector<std::pair<std::string, std::string>> data)
{
    data_base =  DataBase::get_DataBase();
    turn = Turn::get_Turn();

    data_base->generate_players(data);
    data_base->generate_map();
    data_base->generate_items();
    turn->next_player();
    initialize();

    delete menu;
    menu = nullptr;
}

void GameInterface::load()
{
    data_base =  DataBase::get_DataBase();
    turn = Turn::get_Turn();

    //вызов метода загрузки
    save_load_manager->load_all();
    is_load = true;
    initialize();

    delete menu;
    menu = nullptr;
}

void GameInterface::inventory_button_clicked()
{
    current_inventory->setVisible(!current_inventory->isVisible());
    current_equipment_slot->setVisible(!current_equipment_slot->isVisible());
}

void GameInterface::next_turn_button_clicked()
{
    turn->next_player();
    next_turn_button->setEnabled(false);
    //inventory_button->setEnabled(true);

    std::string name = turn->get_player()->get_name();
    players_name->setText("Имя: " + QString::fromStdString(name));

    roll_button->setEnabled(true);
    current_map->clear_chosen_way();
    current_inventory->setVisible(false);
    current_inventory = inventories[(turn->get_turn_number()-1) % inventories.size()];

    current_equipment_slot->setVisible(false);
    current_equipment_slot = equipment_slots[(turn->get_turn_number()-1) % equipment_slots.size()];

    action->set_text(""); // делает окно действий пустым

    //вызов метода сохранения
    save_load_manager->save_all();

    update_labels();
}

void GameInterface::roll_button_clicked()
{
    turn->dice_roll();
    int roll = turn->get_roll();
    players_roll->setText("Шагов: " + QString::number(roll));
    roll_button->setEnabled(false);
    action->set_text("Вы бросили кубики и выкинули: " + QString::number(roll));
    current_map->want_to_move();
}

void GameInterface::enable_next_button()
{
    next_turn_button->setEnabled(true);
}

void GameInterface::add_item(Equipment *item)
{
    current_inventory->add_new_item(item);
}

void GameInterface::pause_button()
{
    pause->setVisible(true);
}

void GameInterface::remaining_rolls()
{
    players_roll->setText("Шагов: " + QString::number(turn->get_roll()));
}

void GameInterface::continue_playing()
{
    pause->setVisible(false);
}

void GameInterface::to_main()
{
    pause->setVisible(false);
    menu = new Menu();
    connect(menu, &Menu::load_the_game, this, &GameInterface::load);
    end_game();
    setCentralWidget(menu);
}

void GameInterface::process_equip(Equipment *item, QString place)
{
    Player* player = turn->get_player();
    if(item->get_class() == "оружие")
        player->equip_weapon(dynamic_cast<Weapon*>(item), place.toStdString());
    else if(item->get_class() == "броня")
        player->equip_armour(dynamic_cast<Armour*>(item), place.toStdString());
    else
        player->equip_jewel(dynamic_cast<Jewel*>(item), place.toStdString());

    update_labels();
}

void GameInterface::process_unequip(Equipment *item, QString place)
{
    turn->get_player()->unequip_item(item, place.toStdString());
    update_labels();
}

void GameInterface::congratulate_the_winner()
{
    CongratulationWindow* clw = new CongratulationWindow(this, QString::fromStdString(turn->get_player()->get_name()));
    setCentralWidget(clw);
    connect(clw, &CongratulationWindow::exit_the_game, this, &QCoreApplication::quit);
}

// восстановление состояния кнопок
void GameInterface::update_buttons()
{
    roll_button->setEnabled(!turn->was_roll());
    next_turn_button->setEnabled(turn->get_already_moved());
    // inventory_button->setEnabled(!turn->get_already_moved()); // надо подумать...
    inventory_button->setEnabled(true);
    menu_button->setEnabled(true);
}

// обновляет конкретный инвентарь и слоты для экипировки по номеру
void GameInterface::update_inventory_and_slots(int id)
{
    if (id < 0 || id >= (int)inventories.size())
    {
        throw std::exception("Index is out of range");
    }

    inventories[id]->update_inventory();
    equipment_slots[id]->update_equiped();
}

// обновляет все инвентари и слоты для экипировки
void GameInterface::update_all_inventories_and_slots()
{
    for (int i = 0; i < (int)inventories.size(); ++i)
    {
        update_inventory_and_slots(i);
        if (turn->get_player() == inventories[i]->get_player())
        {
            current_inventory = inventories[i];
            current_equipment_slot = equipment_slots[i];
        }
    }
}

// обновляет все лейблы
void GameInterface::update_labels()
{
    players_roll->setText("Шагов: " + QString::number(turn->get_roll()));

    // см. дефайн
    players_health_points->setText("ОЗ: " + QString::number(get_plchar("HP")));
    players_attack->setText("Атака: " + QString::number(get_plchar("ATK")));
    players_armour->setText("Броня: " + QString::number(get_plchar("ARM")));
}

// обновляет карту
void GameInterface::update_map()
{
    delete current_map;
    delete mini_map;

    current_map = new GameMap(this);
    current_map->setGeometry(10, 10, 0.79 * screen_size.width(), 0.9375 * screen_size.height());
    current_map->setVisible(true);
    current_map->lower();

    connect(current_map, &GameMap::can_finish_turn, this, &GameInterface::enable_next_button);
    connect(current_map, &GameMap::update_roll, this, &GameInterface::remaining_rolls);
    connect(current_map, &GameMap::win_by_killing, this, &GameInterface::congratulate_the_winner);
    connect(current_map, &GameMap::action, action, &ActionWindow::set_text);
    connect(current_map, &GameMap::event_triggered, this, &GameInterface::process_event_start);

    mini_map = new MiniMap(this, current_map);
    mini_map->setGeometry(0.8 * screen_size.width(), 0.6 * screen_size.height(), 0.195 * screen_size.width(), 0.347 * screen_size.height());
    mini_map->setVisible(true);
    mini_map->lower();
}

// обновляет всё вышеперечисленное
void GameInterface::update_all()
{
    update_buttons();
    update_all_inventories_and_slots();
    update_labels();
    update_map();
}

// обновляет инвентарь, слоты экипировки, лэйблы текщуго игрока
void GameInterface::update_player_status()
{
    update_inventory_and_slots(turn->get_player()->get_id() - 1); // айдишники начинаются с 1
    turn->get_player()->update_chars();
    update_labels();
    update_buttons();
}

void GameInterface::process_event_start()
{
    Event_window *event_window = new Event_window(this, turn->get_player(), turn->get_activated_event());

    // здесь можно было приконектить к методу, который бы отдельно обработал конец ивента и затем отправлял сигнал,
    // связанный с обработкой подбора предмета (условно не вывод в action, а отдельное окно с изображением полученного предмета)
    // однако пока что такого окна нету, поэтому можно скипнуть, сразу привязав конец ивента к подбору предмета
    connect(event_window, &Event_window::event_ended, this, &GameInterface::process_item_pick);

    roll_button->setEnabled(false);
    next_turn_button->setEnabled(false);
    inventory_button->setEnabled(false);
    menu_button->setEnabled(false);
    event_window->setVisible(true);
}

void GameInterface::process_item_pick() // совершает подбор предмета, по совместительству заканичивает цепочку действий после конца движения
{
        if(turn->get_picked_item())
        {
            add_item(turn->get_picked_item());
            action->set_text("Вы получили предмет: " + QString::fromStdString(turn->get_picked_item()->get_name()) + " (предмет тайла)");
        }

        update_player_status();
}
