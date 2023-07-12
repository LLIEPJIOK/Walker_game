
#include "game_interface.h"
#include <fstream>
#include "Menu/general.h"
#include <QApplication>
Game_Interface::Game_Interface(QWidget *parent)
    : QMainWindow{parent}
{
    current_map = nullptr;
    mini_map = nullptr;
    is_load = false;

    data_base =  DataBase::get_DataBase();
    turn  = Turn::get_Turn();
    screen_size = QApplication::screens().at(0)->size();


    setFixedSize(screen_size);
    showFullScreen();
    menu = new Menu();
    setCentralWidget(menu);

    //создание нового объекта класса SaveAndLoadManager
    save_load_manager = new SaveAndLoadManager("SAVE_FILE");

    connect(General::get_general(), &General::start_game, this, &Game_Interface::start);
    connect(menu, &Menu::load_the_game, this, &Game_Interface::load);
}

void Game_Interface::update_characteristics()
{
    int roll = turn->get_roll();
    players_roll->setText("Шагов: " + QString::number(roll));

    int health_points = turn->get_player()->get_characteristics()["HP"];
    players_health_points->setText("ОЗ: " + QString::number(health_points));

    int attack = turn->get_player()->get_characteristics()["ATK"];
    players_attack->setText("Атака: " + QString::number(attack));

    int armour = turn->get_player()->get_characteristics()["ARM"];
    players_armour->setText("Броня: " + QString::number(armour));
}

Game_Interface::~Game_Interface()
{
    delete turn;
    delete data_base;

    foreach(Inventory* inv, inventories)
        delete inv;
    inventories.clear();
    foreach(EquipedItems* ei, wins)
        delete ei;
    wins.clear();
    delete current_map;
    delete mini_map;
    delete menu;
}

void Game_Interface::initialize()
{
    QFont font ("Arial", 14, QFont::Normal, 1);
    QString style("color: rgb(255, 255, 255)");
    current_map = new GameMap(this);
    current_map->setGeometry(10, 10, 0.79 * screen_size.width(), 0.9375 * screen_size.height());
    connect(current_map, &GameMap::can_finish_turn, this, &Game_Interface::enable_next_button);
    connect(current_map, &GameMap::item_was_picked, this, &Game_Interface::add_item);
    connect(current_map, &GameMap::update_roll, this, &Game_Interface::remaining_rolls);
    connect(current_map, &GameMap::win_by_killing, this, &Game_Interface::congratulate_the_winner);

    current_map->setVisible(true);

    mini_map = new MiniMap(this, current_map);
    mini_map->setGeometry(0.8 * screen_size.width(), 0.6 *screen_size.height(), 0.195 * screen_size.width(), 0.347*screen_size.height());
    mini_map->setVisible(true);

    menu_button = new QPushButton("| |",this);
    menu_button->setGeometry(0.8 * screen_size.width(), 10, 0.195 * screen_size.width(), 0.069 * screen_size.height());
    connect(menu_button, &QPushButton::clicked, this, &Game_Interface::pause_button);
    menu_button->setFlat(1);
    menu_button->setFont(font);
    menu_button->setStyleSheet(style);
    menu_button->setVisible(true);

    roll_button = new QPushButton("Roll", this);
    roll_button->setGeometry(0.8 * screen_size.width(), 10 + 0.069 * screen_size.height(), 0.195 * screen_size.width(), 0.069 * screen_size.height());
    roll_button->setFlat(1);
    roll_button->setFont(font);
    roll_button->setStyleSheet(style);
    connect(roll_button, &QPushButton::clicked, this, &Game_Interface::roll_button_clicked);
    roll_button->setVisible(true);

    next_turn_button = new QPushButton("End Turn", this);
    next_turn_button-> setGeometry(0.8 * screen_size.width(), 10 + 0.138 * screen_size.height(), 0.195 * screen_size.width(), 0.069 * screen_size.height());
    next_turn_button->setFlat(1);
    next_turn_button->setFont(font);
    next_turn_button->setStyleSheet(style);
    connect(next_turn_button, &QPushButton::clicked, this, &Game_Interface::next_turn_button_clicked);
    next_turn_button->setVisible(true);
    next_turn_button->setEnabled(false);

    inventory_button = new QPushButton("Inventory", this);
    inventory_button->setGeometry(0.8 * screen_size.width(), 10 + 0.207 * screen_size.height(), 0.195 * screen_size.width(), 0.069 * screen_size.height());
    inventory_button->setFlat(1);
    inventory_button->setFont(font);
    inventory_button->setStyleSheet(style);
    inventory_button->setVisible(true);
    connect(inventory_button, &QPushButton::clicked, this, &Game_Interface::inventory_button_clicked);

    action = new ActionWindow(this);
    action->setGeometry(0.8 * screen_size.width(), 10 + 0.276 * screen_size.height(), 0.195 * screen_size.width(), 0.324 * screen_size.height() - 10);
    action->setVisible(true);
    connect(current_map, &GameMap::action, action, &ActionWindow::set_text);

    for(auto& i: *data_base->get_sequence())
    {
        Inventory* inventory = new Inventory(this);
        inventory->setVisible(false);
        inventories.push_back(inventory);
        connect(inventory, &Inventory::item_was_equiped, this, &Game_Interface::process_equip);
        connect(inventory, &Inventory::item_was_unequiped, this, &Game_Interface::process_unequip);
        if(is_load)
        {
            for(auto& j : *i->get_armourment())
                inventory->add_new_item(j);
            for(auto& j : *i->get_weaponary())
                inventory->add_new_item(j);
            for(auto& j : *i->get_jewellery())
                inventory->add_new_item(j);
            for(auto& j : *i->get_potions())
                inventory->add_new_item(j);
        }
    }

    current_inventory = inventories[0];

    for(int i = 0; i < data_base->get_sequence()->size(); i++)
    {
        EquipedItems* win = new EquipedItems(this);
        win->setVisible(false);
        win->setGeometry(0.335 * screen_size.width(), 20, 0, 0);
        wins.push_back(win);
    }

    current_win = wins[0];


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


    update_characteristics();
    pause = new PauseMenu(this);
    pause->setVisible(false);
    connect(pause, &PauseMenu::continue_button_clicked, this, &Game_Interface::continue_playing);
    connect(pause, &PauseMenu::main_menu_clicked, this, &Game_Interface::to_main);
}

void Game_Interface::end_game()
{
    delete turn;
    turn = nullptr;
    delete data_base;
    data_base = nullptr;

    foreach(Inventory* inv, inventories)
        delete inv;
    inventories.clear();
    foreach(EquipedItems* ei, wins)
        delete ei;
    wins.clear();
    delete current_map;
    current_map = nullptr;
    delete mini_map;
    mini_map = nullptr;
}

void Game_Interface::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(44, 66, 47)));
    painter.setPen(QPen(QColor(44, 66, 47)));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRect(0, 0 , this->width(), this->height());
    QMainWindow::paintEvent(event);
}

void Game_Interface::start(std::vector<std::pair<std::string, std::string>> data)
{
    data_base->generate_players(data);
    data_base->generate_map();
    data_base->generate_items();
    turn->next_player();
    initialize();
    delete menu;
    menu = nullptr;
}

void Game_Interface::load()
{
    //вызов метода загрузки
    save_load_manager->load_all();
    is_load = true;
    initialize();
    delete menu;
    menu = nullptr;
}

void Game_Interface::inventory_button_clicked()
{
    current_inventory->setVisible(!current_inventory->isVisible());
    current_win->setVisible(!current_win->isVisible());
}

void Game_Interface::next_turn_button_clicked()
{
    turn->next_player();
    next_turn_button->setEnabled(false);

    std::string name = turn->get_player()->get_name();
    players_name->setText("Имя: " + QString::fromStdString(name));

    roll_button->setEnabled(true);
    current_map->clear_chosen_way();
    current_inventory->setVisible(false);
    current_inventory = inventories[(turn->get_turn_number()-1) % inventories.size()];

    current_win->setVisible(false);
    current_win = wins[(turn->get_turn_number()-1) % wins.size()];

    //вызов метода сохранения
    save_load_manager->save_all();

    update_characteristics();
}

void Game_Interface::roll_button_clicked()
{
    turn->dice_roll();
    int roll = turn->get_roll();
    players_roll->setText("Шагов: " + QString::number(roll));
    roll_button->setEnabled(false);
    action->set_text("Вы бросили кубики и выкинули: " + QString::number(roll));
    current_map->want_to_move();
}

void Game_Interface::enable_next_button()
{
    next_turn_button->setEnabled(true);
}

void Game_Interface::add_item(Equipment *item)
{
    current_inventory->add_new_item(item);
}

void Game_Interface::pause_button()
{
    pause->setVisible(true);
}

void Game_Interface::remaining_rolls()
{
    players_roll->setText("Шагов: " + QString::number(turn->get_roll()));
}

void Game_Interface::continue_playing()
{
    pause->setVisible(false);
}

void Game_Interface::to_main()
{
    pause->setVisible(false);
    menu = new Menu();
    end_game();
    setCentralWidget(menu);
}

void Game_Interface::process_equip(Equipment *item, QString place)
{
    Player* player = turn->get_player();
    if(item->get_class() == "оружие")
        player->equip_weapon(dynamic_cast<Weapon*>(item), place.toStdString());
    else if(item->get_class() == "броня")
        player->equip_armour(dynamic_cast<Armour*>(item), place.toStdString());
    else
        player->equip_jewel(dynamic_cast<Jewel*>(item), place.toStdString());
    update_characteristics();
}

void Game_Interface::process_unequip(Equipment *item, QString place)
{
    turn->get_player()->unequip_item(item, place.toStdString());
    update_characteristics();
}

void Game_Interface::congratulate_the_winner()
{
    CongratulationWindow* clw = new CongratulationWindow(this, QString::fromStdString(turn->get_player()->get_name()));
    setCentralWidget(clw);
    connect(clw, &CongratulationWindow::exit_the_game, this, &QCoreApplication::quit);
}

