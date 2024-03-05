#include "Turn.h"
#include "DataBase.h"
#include "Engine/Transceiver.h"

#define seq DataBase::get_DataBase()->get_sequence()
#define db DataBase::get_DataBase()
#define sleep std::this_thread::sleep_for(std::chrono::milliseconds(500));

Turn* Turn::turn = nullptr;


Turn::Turn()
{
    turn_number = 0;
    game_over = false;
    player = nullptr;
    picked_item = nullptr;
    activated_event = nullptr;
}

Turn *Turn::get_Turn()
{
    if(!turn)
        turn = new Turn();
    return turn;
}

Turn::~Turn()
{
    turn = 0;
}

void Turn::dice_roll()
{
    srand(time(NULL));
    int DQNT = player->get_characteristics()["DQNT"];
    roll =  rand() % (5 * DQNT + 1) + DQNT + player->get_characteristics().at("ROLL_MOD");
    rolled = roll;
}

Player* Turn::get_player()
{
    return player;
}

void Turn::set_player(Player *_player)
{
    player = _player;
}

int Turn::get_roll()
{
    return roll;
}

int Turn::get_rolled()
{
    return rolled;
}

bool Turn::get_moving()
{
    return is_moving;
}

bool Turn::get_already_moved()
{
    return has_already_moved;
}

bool Turn::get_game_over()
{
    return game_over;
}

void Turn::set_already_moved(bool moved)
{
    has_already_moved = moved;
}

bool Turn::get_has_attacked()
{
    return has_attacked;
}

bool Turn::event_is_finished()
{
    return event_finished;
}

void Turn::set_event_is_finished(bool finished)
{
    event_finished = finished;
}

void Turn::set_has_attacked(bool atk)
{
    has_attacked = atk;
}

void Turn::set_game_over(bool over)
{
    game_over = over;
}

void Turn::set_roll(int _roll)
{
    roll = _roll;
}

void Turn::set_is_moving(bool moving)
{
    is_moving = moving;
}


void Turn::set_turn_number(int _turn_number)
{
    turn_number = _turn_number;
}

void Turn::process_end_of_movement()
{
    auto map = DataBase::get_DataBase()->get_graph_map()->getMap();
    auto players_position = player->get_players_position();

    if(map[players_position].get_item() != "None") {
        game_msg msg = {player->get_id() - 1, 0, 6, 0};
        std::string buff = map[players_position].get_item();
        for (int i = 0; i < 127 && i < buff.size(); i++)
            msg.buffer[i] = buff[i];

        Transceiver::get_transceiver()->send_msg(msg);

        picked_item = player->add_item(map[players_position].get_item());
        map[players_position].set_item("None");
        }

    if(map[players_position].get_event_name() != "???")
        activated_event = Events::get_Events()->get_events()->at(map[players_position].get_event_name());

    has_already_moved = true;
    is_moving = false;
}

Equipment *Turn::get_picked_item()
{
    return picked_item;
}

Event *Turn::get_activated_event()
{
    return activated_event;
}

int Turn::get_turn_number()
{
    return turn_number;
}

void Turn::save(QFile &out)
{
    //запись turn_number
    out.write((char*)& turn_number, sizeof(turn_number));

    //запись game_over
    out.write((char*)& game_over, sizeof(game_over));

    //запись roll
    out.write((char*)& roll, sizeof(roll));

    // запись rolled
    out.write((char*)& rolled, sizeof(rolled));

    //запись is_moving
    out.write((char*)& is_moving, sizeof(is_moving));

    //запись has_already_moved
    out.write((char*)& has_already_moved, sizeof(has_already_moved));

    //запись has_attacked
    out.write((char*)& has_attacked, sizeof(has_attacked));

    //запись event_finished
    out.write((char*)& event_finished, sizeof(event_finished));
}

void Turn::load(QFile &in)
{
    //чтение turn_number
    in.read((char*)&turn_number, sizeof(turn_number));

    //чтение game_over
    in.read((char*)&game_over, sizeof(game_over));

    //чтение roll
    in.read((char*)&roll, sizeof(roll));

    // чтение rolled
    in.read((char*)&rolled, sizeof(rolled));

    //чтение is_moving
    in.read((char*)&is_moving, sizeof(is_moving));

    //чтение has_already_moved
    in.read((char*)&has_already_moved, sizeof(has_already_moved));

    //чтение has_attacked
    in.read((char*)&has_attacked, sizeof(has_attacked));

    //чтение event_finished
    in.read((char*)&event_finished, sizeof(event_finished));

    //присваивание указателя на объект класса Player в зависимости от переменной turn_number
    player = seq->at((turn_number - 1) % seq->size());
}

bool Turn::was_roll()
{
    return roll != 0 || is_moving || has_already_moved;
}


void Turn::next_player()
{
    if(turn->turn_number != 0)
    {
        player->process_active_effects();
        player->set_killed_player(-1);
    }
    player = seq->at(turn_number % seq->size());
    turn_number++;
    roll = 0;
    is_moving = false;
    has_already_moved = false;
    has_attacked = false;
    picked_item = nullptr;
    activated_event = nullptr;
    event_finished = 1;
};
