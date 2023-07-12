#include "Turn.h"
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

}

void Turn::dice_roll()
{
    srand(time(NULL));
    int DQNT = player->get_characteristics()["DQNT"];
    roll =  rand() % (5 * DQNT + 1) + DQNT;
}

Player* Turn::get_player() {  return player; }

void Turn::set_player(Player *_player)
{
    player = _player;
}

int Turn::get_roll() { return roll; }

bool Turn::get_moving() { return is_moving; }

bool Turn::get_already_moved() { return has_already_moved; }

bool Turn::get_game_over()
{
    return game_over;
}

void Turn::set_already_moved(bool moved) { has_already_moved = moved; }

bool Turn::get_has_attacked() { return has_attacked; }

bool Turn::event_is_finished()
{
    return event_finished;
}

void Turn::set_event_is_finished(bool finished)
{
    event_finished = finished;
}

void Turn::set_has_attacked(bool atk) { has_attacked = atk; }

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

void Turn::set_start_x(int _start_x)
{
    start_x = _start_x;
}

void Turn::set_start_y(int _start_y)
{
    start_y = _start_y;
}

void Turn::set_turn_number(int _turn_number)
{
    turn_number = _turn_number;
}

Equipment *Turn::get_picked_item()
{
    return picked_item;
}

Event *Turn::get_activated_event()
{
    return activated_event;
}

std::pair<int, int> *Turn::get_chosen_direction()
{
    return &chosen_direction;
}

int Turn::get_start_x()
{
    return start_x;
}

int Turn::get_start_y()
{
    return start_y;
}

int Turn::get_turn_number() { return turn_number; }

void Turn::set_chosen_direction(int x, int y)
{
    chosen_direction.first = x;
    chosen_direction.second = y;
}

std::vector<std::pair<int, int>> Turn::find_possible_ways()
{
    MapCell** map = DataBase::get_DataBase()->get_map();
    std::vector<std::pair<int, int>> ways;
    int posy = player->get_y();
    int posx = player->get_x();

    if(player->get_y()!= 0)
        if (map[posy - 1][posx].get_type_of_terrain() != "non_moving_area" && player->get_previous_direction() != std::make_pair(0 ,-1))
            ways.push_back(std::make_pair(posx, posy - 1));
    if(player->get_x() != DataBase::get_DataBase()->get_width()-1)
        if (map[posy][posx + 1].get_type_of_terrain() != "non_moving_area" && player->get_previous_direction() != std::make_pair(1, 0))
            ways.push_back(std::make_pair(posx + 1, posy));
    if(player->get_y() != DataBase::get_DataBase()->get_height()-1)
        if (map[posy + 1][posx].get_type_of_terrain() != "non_moving_area" && player->get_previous_direction() != std::make_pair(0 , 1))
            ways.push_back(std::make_pair(posx, posy + 1));;
    if(player->get_x() != 0)
        if (map[posy][posx - 1].get_type_of_terrain() != "non_moving_area" && player->get_previous_direction() != std::make_pair(-1, 0))
            ways.push_back(std::make_pair(posx - 1, posy));
    return ways;
}

std::set<Player *> Turn::check_players_in_range() const
{
    std::set<Player*> s;
    for (auto i = seq->begin(); i != seq->end(); ++i)
        if ((*i)->get_id() != player->get_id() && abs((*i)->get_x() - player->get_x()) + abs((*i)->get_y() - player->get_y()) <= player->get_characteristics()["RNG"])
            s.insert(*i);
    return s;
}

void Turn::save(std::ofstream &out)
{
    //запись tuen_number
    out.write((char*)& turn_number, sizeof(turn_number));

    //запись game_over
    out.write((char*)& game_over, sizeof(game_over));

    //запись roll
    out.write((char*)& roll, sizeof(roll));

    //запись is_moving
    out.write((char*)& is_moving, sizeof(is_moving));

    //запись has_already_moved
    out.write((char*)& has_already_moved, sizeof(has_already_moved));

    //запись has_attacked
    out.write((char*)& has_attacked, sizeof(has_attacked));

    //запись event_finished
    out.write((char*)& event_finished, sizeof(event_finished));

    //запись start_x
    out.write((char*)& start_x, sizeof(start_x));

    //запись start_y
    out.write((char*)& start_y, sizeof(start_y));

    //запись первого значения chosen_direction
    out.write((char*)& chosen_direction.first, sizeof(chosen_direction.first));
    //запись второго значения chosen_direction
    out.write((char*)& chosen_direction.second, sizeof(chosen_direction.second));
}

void Turn::load(std::ifstream &in)
{
    //чтение turn_number
    in.read((char*)& turn_number, sizeof(turn_number));

    //чтение game_over
    in.read((char*)& game_over, sizeof(game_over));

    //чтение roll
    in.read((char*)& roll, sizeof(roll));

    //чтение is_moving
    in.read((char*)& is_moving, sizeof(is_moving));

    //чтение has_already_moved
    in.read((char*)& has_already_moved, sizeof(has_already_moved));

    //чтение has_attacked
    in.read((char*)& has_attacked, sizeof(has_attacked));

    //чтение event_finished
    in.read((char*)& event_finished, sizeof(event_finished));

    //чтение start_x
    in.read((char*)& start_x, sizeof(start_x));

    //чтение start_y
    in.read((char*)& start_y, sizeof(start_y));

    //чтение первого значения chosen_direction
    in.read((char*)& chosen_direction.first, sizeof(chosen_direction.first));
    //чтение второго значения chosen_direction
    in.read((char*)& chosen_direction.second, sizeof(chosen_direction.second));

    //присваивание указателя на объект класса Player в зависимости от переменной turn_number
    player = seq->at((turn_number - 1)%seq->size());
}

std::vector<std::pair<int, int>> Turn::move_player()
{
    is_moving = true;
    std::vector<std::pair<int, int>> ways = find_possible_ways();
    if(chosen_direction == std::make_pair(-1, -1))
        return ways;
    if(roll>0)
    {
        roll--;
        player->set_previous_direction(std::make_pair(player->get_x()-chosen_direction.first, player->get_y() - chosen_direction.second));
        player->set_x(chosen_direction.first);
        player->set_y(chosen_direction.second);
        ways = find_possible_ways();
        if(ways.size() == 1 && ways[0] != chosen_direction)
        {
            chosen_direction = ways[0];
        }
        if(roll)
            return ways;
    }
    has_already_moved = true;
    //change_player_position(start_x, start_y, player->get_x(), player->get_y());
    MapCell **map = DataBase::get_DataBase()->get_map();

    if(map[player->get_y()][player->get_x()].get_item() != "Нет")
    {
        picked_item = player->add_item(map[player->get_y()][player->get_x()].get_item());
        map[player->get_y()][player->get_x()].set_item("Нет");
    }
    if(map[player->get_y()][player->get_x()].get_event_name()!="???")
        activated_event = Events::get_Events()->get_events()->at("experiment");
    is_moving = false;
    return std::vector<std::pair<int, int>>();
}

void Turn::next_player()
{
    if(turn->turn_number!=0)
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
    start_x = player->get_x();
    start_y = player->get_y();
    chosen_direction = std::make_pair(-1, -1);
};
