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
    turn = nullptr;
    data_base = nullptr;

    is_load = false;
    game_is_played = false;
    game_is_paused = false;

    screen_size = QApplication::screens().at(0)->size();

    setFixedSize(screen_size);
    showFullScreen();

    key_to_action[Qt::Key_I] = &GameInterface::inventory_button_clicked;
    key_to_action[Qt::Key_Escape] = &GameInterface::pause_button;
    key_to_action[Qt::Key_Q] = &GameInterface::show_player;

    menu = new Menu();
    setCentralWidget(menu);

    information_window = new InformationWindow(this);

    //создание нового объекта класса SaveAndLoadManager
    save_load_manager = new SaveAndLoadManager("SAVE_FILE");

    connect(General::get_general(), &General::start_game, this, &GameInterface::start);
    connect(General::get_general(), &General::load_game, this, &GameInterface::load);
}

GameInterface::~GameInterface()
{
    delete information_window;
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

    foreach (Player_status_widget* st, players_statuses) {
        delete st;
    }

    players_statuses.clear();
}

void GameInterface::initialize()
{
    QFont font ("Arial", 14, QFont::Normal, 1);
    QString style("color: rgb(255, 255, 255)");

    for(int i = 0; i < 5; i++)
    {
        buttons.push_back(new QPushButton(this));
        buttons[i]->setGeometry(0.8 * screen_size.width(), 10 + i * 0.069 * screen_size.height(), 0.195 * screen_size.width(), 0.069 * screen_size.height());
        buttons[i]->setFlat(1);
        buttons[i]->setFont(font);
        buttons[i]->setStyleSheet(style);
        buttons[i]->setVisible(true);
    }

    buttons[0]->setText("| |");
    connect(buttons[0], &QPushButton::clicked, this, &GameInterface::pause_button);

    buttons[1]->setText("Roll");
    connect(buttons[1], &QPushButton::clicked, this, &GameInterface::roll_button_clicked);

    buttons[2]->setText("End Turn");
    connect(buttons[2], &QPushButton::clicked, this, &GameInterface::next_turn_button_clicked);

    buttons[3]->setText("Inventory");
    connect(buttons[3], &QPushButton::clicked, this, &GameInterface::inventory_button_clicked);

    buttons[4]->setText("Status");
    connect(buttons[4], &QPushButton::clicked, this, &GameInterface::status_button_clicked);


    action = new ActionWindow(this);
    action->setGeometry(0.8 * screen_size.width(), 10 + 0.345 * screen_size.height(), 0.195 * screen_size.width(), 0.255 * screen_size.height() - 10);
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

    for(auto& i: *data_base->get_sequence())
    {
        Player_status_widget* status = new Player_status_widget(this, i);
        status->setVisible(false);
        players_statuses.push_back(status);
    }

    pause = new PauseMenu(this);
    pause->setVisible(false);
    connect(pause, &PauseMenu::continue_button_clicked_signal, this, &GameInterface::pause_button);
    connect(pause, &PauseMenu::main_menu_clicked, this, &GameInterface::to_main);
    connect(pause, &PauseMenu::save_game_signal, this, &GameInterface::save_game);

    update_all();
    current_player_status->update_all();
    game_is_played = true;
}

void GameInterface::show_player()
{
    current_map->move_to_player();
}

void GameInterface::end_game()
{
    game_is_played = false;

    delete turn;
    turn = nullptr;
    delete data_base;
    data_base = nullptr;
    delete pause;
    pause = nullptr;

    foreach(Inventory* inv, inventories)
        delete inv;

    inventories.clear();
    //inventories.shrink_to_fit();

    foreach(EquipedItems* ei, equipment_slots)
        delete ei;
    equipment_slots.clear();
    //equipment_slots.shrink_to_fit();

    foreach (Player_status_widget* w, players_statuses) {
        delete w;
    }
    players_statuses.clear();

    delete current_map;
    current_map = nullptr;
    delete mini_map;
    mini_map = nullptr;

    for(int i = 0; i < buttons.size(); i++)
    {
        delete buttons[i];
        buttons[i] = nullptr;
    }
    buttons.clear();
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

void GameInterface::keyPressEvent(QKeyEvent *event)
{
    if(key_to_action.contains(event->key()) && game_is_played)
    {
        (this->*key_to_action[event->key()])();
    }
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
}

void GameInterface::load(QString file_name)
{
    data_base =  DataBase::get_DataBase();
    turn = Turn::get_Turn();

    //вызов метода загрузки
    save_load_manager->set_file_name(file_name);
    save_load_manager->load_all();
    is_load = true;
    initialize();
}

void GameInterface::inventory_button_clicked()
{
    current_inventory->setVisible(!current_inventory->isVisible());
    current_equipment_slot->setVisible(!current_equipment_slot->isVisible());
}

void GameInterface::next_turn_button_clicked()
{
    turn->next_player();
    buttons[2]->setEnabled(false);
    //inventory_button->setEnabled(true);

    buttons[1]->setEnabled(true);
    current_map->clear_chosen_way();
    current_inventory->setVisible(false);
    current_inventory = inventories[(turn->get_turn_number()-1) % inventories.size()];

    current_player_status->setVisible(false);
    current_player_status = players_statuses[(turn->get_turn_number()-1) % players_statuses.size()];

    current_equipment_slot->setVisible(false);
    current_equipment_slot = equipment_slots[(turn->get_turn_number()-1) % equipment_slots.size()];

    action->set_text(""); // делает окно действий пустым

    update_player_status();
    current_map->move_to_player();
}

void GameInterface::roll_button_clicked()
{
    turn->dice_roll();
    int roll = turn->get_roll();
    action->set_text("Вы бросили кубики и выкинули: " + QString::number(roll));
    current_map->want_to_move();
}

void GameInterface::status_button_clicked()
{
    current_player_status->setVisible(!current_player_status->isVisible());
}

void GameInterface::enable_next_button()
{
    buttons[2]->setEnabled(true);
}

void GameInterface::add_item(Equipment *item)
{
    current_inventory->add_new_item(item);
}

void GameInterface::pause_button()
{
    pause->setVisible(!pause->isVisible());
}

void GameInterface::to_main()
{
    pause->setVisible(false);
    menu = new Menu();
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

    current_player_status->update_all();
}

void GameInterface::process_unequip(Equipment *item, QString place)
{
    turn->get_player()->unequip_item(item, place.toStdString());
    current_player_status->update_all();
}

void GameInterface::congratulate_the_winner()
{
    CongratulationWindow* clw = new CongratulationWindow(this, QString::fromStdString(turn->get_player()->get_name()));
    setCentralWidget(clw);
    connect(clw, &CongratulationWindow::exit_the_game, this, &QCoreApplication::quit);
}

void GameInterface::save_game(QString file_name)
{
    save_load_manager->set_file_name(file_name);
    save_load_manager->save_all();
    information_window->raise();
    information_window->inform("Игра сохранилась");
}

void GameInterface::all_is_ready()
{
    if(is_load && turn->get_roll())
        current_map->want_to_move();
    menu->setVisible(false);
    delete menu;
    menu = nullptr;
}

// восстановление состояния кнопок
void GameInterface::update_buttons()
{
    buttons[1]->setEnabled(!turn->was_roll());
    buttons[2]->setEnabled(turn->get_already_moved());
    // inventory_button->setEnabled(!turn->get_already_moved()); // надо подумать...
    buttons[3]->setEnabled(true);
    buttons[0]->setEnabled(true);
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
            current_player_status = players_statuses[i];
        }
    }
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
    connect(current_map, &GameMap::win_by_killing, this, &GameInterface::congratulate_the_winner);
    connect(current_map, &GameMap::action, action, &ActionWindow::set_text);
    connect(current_map, &GameMap::event_triggered, this, &GameInterface::process_event_start);
    connect(current_map, &GameMap::was_initialized, this, &GameInterface::all_is_ready);

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
    update_map();
}

// обновляет инвентарь, слоты экипировки, лэйблы текщуго игрока
void GameInterface::update_player_status()
{
    Player* tmp = turn->get_player();
    update_buttons();
    update_inventory_and_slots(tmp->get_id() - 1); // айдишники начинаются с 1
    tmp->update_chars();
    tmp->die();
    current_player_status->update_all();
}

void GameInterface::process_event_start()
{
    Event_window *event_window = new Event_window(this, turn->get_player(), turn->get_activated_event());
    event_window->setGeometry(screen_size.width() / 2 - event_window->width() / 2, screen_size.height() / 2 - event_window->height() / 2, 0, 0);
    // здесь можно было приконектить к методу, который бы отдельно обработал конец ивента и затем отправлял сигнал,
    // связанный с обработкой подбора предмета (условно не вывод в action, а отдельное окно с изображением полученного предмета)
    // однако пока что такого окна нету, поэтому можно скипнуть, сразу привязав конец ивента к подбору предмета
    connect(event_window, &Event_window::event_ended, this, &GameInterface::process_item_pick);

    for(int i = 0; i < 4; i++)
    {
        buttons[i]->setEnabled(false);
    }
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
