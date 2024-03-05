#include "GameInterface/general_info_widget.h"
#include "congratulationwindow.h"
#include "game_interface.h"
#include "Menu/general.h"
#include "Events/event_window.h"
#include "Engine/translator.h"

#include <QApplication>

#define get_plchar(character) turn->get_player()->get_characteristics().at(character)
#define sleep(time) std::this_thread::sleep_for(std::chrono::milliseconds(time))


GameInterface::GameInterface(QWidget *parent)
    : QMainWindow{parent}
{
    font = QFont("Arial", 14, QFont::Normal, 1);
    style = QString("color: rgb(255, 255, 255)");

    turn = nullptr;
    data_base = nullptr;

    is_load = false;
    game_is_played = false;
    game_is_paused = false;

    hex_map = nullptr;
    mini_map = nullptr;

    screen_size = QApplication::screens().at(0)->size();

    setFixedSize(screen_size);
    showFullScreen();

    key_to_action[Qt::Key_I] = &GameInterface::info_button_clicked;
    key_to_action[Qt::Key_Escape] = &GameInterface::pause_button;

    menu = new Menu();
    setCentralWidget(menu);

    information_window = new InformationWindow(this);

    //создание нового объекта класса SaveAndLoadManager
    save_load_manager = new SaveAndLoadManager("SAVE_FILE");

    connect(General::get_general(), &General::start_game, this, &GameInterface::start);
    connect(General::get_general(), &General::load_game, this, &GameInterface::load);

    connect(Transceiver::get_transceiver(), &Transceiver::send_cell_data, this, &GameInterface::set_cell_data);
    connect(Transceiver::get_transceiver(), &Transceiver::add_item, this, &GameInterface::add_item_m);
    connect(Transceiver::get_transceiver(), &Transceiver::apply_effect, this, &GameInterface::apply_effect);
    connect(Transceiver::get_transceiver(), &Transceiver::next_turn, this, &GameInterface::next_turn);
    connect(Transceiver::get_transceiver(), &Transceiver::equip, this, &GameInterface::equip_item);
}

GameInterface::~GameInterface()
{
    delete information_window;
    delete turn;
    delete data_base;
    delete menu;

    delete current_info_widget;
}

void GameInterface::initialize()
{
    create_buttons();

    action = new ActionWindow(this);
    action->setGeometry(0.8 * screen_size.width(), 10 + 0.275 * screen_size.height(), 0.195 * screen_size.width(), 0.5 * screen_size.height() - 10);
    action->setVisible(true);

    current_info_widget = new General_info_widget(this, data_base->get_sequence()->at(Transceiver::get_transceiver()->get_id()));

    pause = new PauseMenu(this);
    pause->setVisible(false);
    connect(pause, &PauseMenu::continue_button_clicked_signal, this, &GameInterface::pause_button);
    connect(pause, &PauseMenu::main_menu_clicked, this, &GameInterface::to_main);
    connect(pause, &PauseMenu::save_game_signal, this, &GameInterface::save_game);

    if (Transceiver::get_transceiver()->get_id() == 0)
        send_map_data();
    update_all();

    game_is_played = true;
}

void GameInterface::create_buttons()
{
    for(int i = 0; i < 4; i++)
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

    buttons[1]->setText(tr("Roll"));
    connect(buttons[1], &QPushButton::clicked, this, &GameInterface::roll_button_clicked);

    buttons[2]->setText(tr("End turn"));
    connect(buttons[2], &QPushButton::clicked, this, &GameInterface::next_turn_button_clicked);

    buttons[3]->setText(tr("Info"));
    connect(buttons[3], &QPushButton::clicked, this, &GameInterface::info_button_clicked);
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

    delete current_info_widget;

    for(int i = 0; i < buttons.size(); i++)
    {
        delete buttons[i];
        buttons[i] = nullptr;
    }
    buttons.clear();
}

void GameInterface::show_player()
{
    hex_map->move_to_player();
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
    if (Transceiver::get_transceiver()->get_id() == 0){
        data_base->generate_events();
        data_base->generate_items();
    }

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
}

void GameInterface::next_turn_button_clicked()
{
    turn->next_player();
    hex_map->next_players_model();
    buttons[2]->setEnabled(false);
    buttons[1]->setEnabled(true);

    action->set_text(""); // делает окно действий пустым
    hex_map->move_to_player();

    game_msg msg = {Transceiver::get_transceiver()->get_id(), 0, 10, 0};
    Transceiver::get_transceiver()->send_msg(msg);

    update_buttons();
    update_player_status();

}

void GameInterface::roll_button_clicked()
{
    // MOVEMENT
    turn->dice_roll();
    int roll = turn->get_roll();
    action->set_text(tr("Your dice roll:") + " " + QString::number(roll));
    hex_map->want_to_move();
    buttons[1]->setEnabled(false);
    buttons[3]->setEnabled(false);
    // NET
}

void GameInterface::info_button_clicked()
{
    current_info_widget->setVisible(!current_info_widget->isVisible());
    current_info_widget->raise();
    //hex_map->setVisible(!hex_map->isVisible());
}

void GameInterface::enable_next_button()
{
    // BUTTONS
    buttons[2]->setEnabled(true);
}

void GameInterface::add_item(Equipment *item)
{
    // INVENTORY
    current_info_widget->addItem(item);
}

void GameInterface::pause_button()
{
    if (current_info_widget->isVisible()) {
        current_info_widget->setVisible(false);
        return;
    }

    current_info_widget->setVisible(false);
    pause->setVisible(!pause->isVisible());
    pause->raise();
}

void GameInterface::to_main()
{
    pause->setVisible(false);
    menu = new Menu();
    end_game();
    setCentralWidget(menu);
}

void GameInterface::congratulate_the_winner()
{
    CongratulationWindow* clw = new CongratulationWindow(this, QString::fromStdString(turn->get_player()->get_name()));
    setCentralWidget(clw);
    connect(clw, &CongratulationWindow::exit_the_game, this, &QCoreApplication::quit);
    // NET
}

void GameInterface::save_game(QString file_name)
{
    save_load_manager->set_file_name(file_name);
    save_load_manager->save_all();
    information_window->raise();
    information_window->inform(tr("Saved!"));
}

void GameInterface::all_is_ready()
{
    if(is_load && turn->get_roll())
        hex_map->want_to_move();
    menu->setVisible(false);
    delete menu;
    menu = nullptr;
    // NET
}

void GameInterface::set_cell_data(game_msg msg)
{
    Location loc;
    loc.q = msg.target_id;
    loc.r = msg.extra;

    int i = 0;
    std::string event;
    for (; i < 127 && msg.buffer[i] != '\\'; i++){
        event.push_back(msg.buffer[i]);
    }

    i++;
    std::string name;
    for (; i < 127 && msg.buffer[i] != '\\'; i++){
        name.push_back(msg.buffer[i]);
    }

    i++;
    std::string item;
    for (; i < 127 && msg.buffer[i] != 0; i++){
        item.push_back(msg.buffer[i]);
    }


    DataBase::get_DataBase()->get_graph_map()->getMap().at(loc).set_event_name(event);
    DataBase::get_DataBase()->get_graph_map()->getMap().at(loc).set_tile_name(name);
    DataBase::get_DataBase()->get_graph_map()->getMap().at(loc).set_item(item);

    qDebug() << event << " " << name << " " << item;
}

void GameInterface::add_item_m(game_msg msg)
{
    std::string item_name;
    for (int i = 0; i < 127 && msg.buffer[i] != 0; i++)
        item_name.push_back(msg.buffer[i]);

    DataBase::get_DataBase()->get_sequence()->at(msg.id)->add_item(item_name);
    action->set_text(QString::fromStdString(DataBase::get_DataBase()->get_sequence()->at(msg.id)->get_name()) + tr(" has picked up an item!") + " " + QString::fromStdString(item_name) + " " + tr("tile item."));

    qDebug() << item_name << " was processed " << DataBase::get_DataBase()->get_sequence()->at(msg.id)->get_name();
}

void GameInterface::apply_effect(game_msg msg)
{
    std::string effect_name;
    for (int i = 0; i < 127 && msg.buffer[i] != 0; i++)
        effect_name.push_back(msg.buffer[i]);

    int duration = msg.extra;
    Effect* eff = new Effect(effect_name,DataBase::get_DataBase()->get_sequence()->at(msg.target_id) , duration);
    eff->apply_effect();

    qDebug() << "Effect " << effect_name << " was applied on " << DataBase::get_DataBase()->get_sequence()->at(msg.target_id)->get_name();
}

void GameInterface::next_turn(game_msg msg)
{
    turn->next_player();
    hex_map->next_players_model();

    if (Transceiver::get_transceiver()->get_id() == turn->get_player()->get_id() - 1)
    {
        buttons[2]->setEnabled(false);
        buttons[1]->setEnabled(true);
    }

    update_buttons();
    update_player_status();

    qDebug() << "Next turn was received!";
}

void GameInterface::unequip_place(game_msg msg)
{
    data_base->get_sequence()->at(msg.target_id)->unequip_item(msg);
    action->set_text(tr("Player ") + QString::fromStdString(data_base->get_sequence()->at(msg.target_id)->get_name()) + " " + tr("has unequipped some item..."));

}

void GameInterface::equip_item(game_msg msg)
{
    data_base->get_sequence()->at(msg.target_id)->equip_item(msg);
    action->set_text(tr("Player ") + QString::fromStdString(data_base->get_sequence()->at(msg.target_id)->get_name()) + " " + tr("has equipped some item..."));
}

// восстановление состояния кнопок
void GameInterface::update_buttons()
{
    if (Transceiver::get_transceiver()->get_id() == turn->get_player()->get_id() - 1)
    {
        buttons[1]->setEnabled(!turn->was_roll());
        buttons[2]->setEnabled(turn->get_already_moved());
        buttons[3]->setEnabled(true);
        buttons[0]->setEnabled(true);
    }
    else {
        buttons[1]->setEnabled(false);
        buttons[2]->setEnabled(false);

        buttons[3]->setEnabled(true);
        buttons[0]->setEnabled(true);
    }
    // NET
}

// обновляет конкретный инвентарь и слоты для экипировки по номеру
void GameInterface::update_info_widget()
{
    current_info_widget->update_all();
}

// обновляет карту
void GameInterface::update_map()
{
    delete hex_map;
    delete mini_map;

    hex_map = new HexMap(this, data_base->get_graph_map());
    hex_map->setGeometry(10, 10, 0.79 * screen_size.width(), 0.9375 * screen_size.height());
    hex_map->setVisible(true);
    hex_map->lower();

    connect(hex_map, &HexMap::can_finish_turn, this, &GameInterface::enable_next_button);
    connect(hex_map, &HexMap::win_by_killing, this, &GameInterface::congratulate_the_winner);
    connect(hex_map, &HexMap::action, action, &ActionWindow::set_text);
    connect(hex_map, &HexMap::event_triggered, this, &GameInterface::process_event_start);
    connect(hex_map, &HexMap::was_initialized, this, &GameInterface::all_is_ready);

    mini_map = new MiniMap(this, hex_map);
    mini_map->setGeometry(0.8 * screen_size.width(), 0.6 * screen_size.height(), 0.195 * screen_size.width(), 0.347 * screen_size.height());
    mini_map->create_moving_area();
    mini_map->setVisible(true);
    mini_map->lower();
}

// обновляет всё вышеперечисленное
void GameInterface::update_all()
{
    update_buttons();
    update_info_widget();
    update_map();
}

void GameInterface::send_map_data()
{
    for (auto& i : DataBase::get_DataBase()->get_graph_map()->getMap()){
        int x = i.first.q;
        int y = i.first.r;
        std::string msg = i.second.get_event_name() + '\\' + i.second.get_tile_name() + '\\' + i.second.get_item();
        game_msg envelope = {0, x, 4, y};

        for (int i = 0; i < 127 && i < msg.size(); i++)
            envelope.buffer[i] = msg[i];

        Transceiver::get_transceiver()->send_msg(envelope);
    }
}

// обновляет инвентарь, слоты экипировки, лэйблы текщуго игрока
void GameInterface::update_player_status()
{
    Player* tmp = turn->get_player();
    update_buttons();
    update_info_widget(); // айдишники начинаются с 1
    tmp->update_chars();
    tmp->die();
    //current_player_status->update_all();
    // NET
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
        action->set_text(tr("You have picked up an item!") + " " + QString::fromStdString(Translator::translate(turn->get_picked_item()->get_name().c_str())) + " " + tr("tile item."));
    }

    update_player_status();
    // NET
}
