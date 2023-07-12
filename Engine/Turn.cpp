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

void Turn::save(std::string file_name)
{
    auto sequence = DataBase::get_DataBase()->get_sequence();
    int height = DataBase::get_DataBase()->get_height();
    int width = DataBase::get_DataBase()->get_width();
    MapCell** map = DataBase::get_DataBase()->get_map();

    std::string save_name = file_name;

    std::ofstream save_file(save_name);
    save_file << "{\n";

    save_file << "\"height\" :" << "\"" + std::to_string(height) + "\"\n";
    save_file << "\"width\" :" << "\"" +  std::to_string(width)+ "\"\n";
    save_file << "\"sequence\" : [\n" ;
    for (const auto& i : *sequence) // for each player
    {
        Player* pl = i;
        save_file << "\"name\" :\n";
        save_file << "{\n"; // saving all the raw player fields
        save_file << "\"name\" :" << "\"" + pl->get_name() + "\"\n";
        save_file << "\"player_id\" :" << "\"" + std::to_string(pl->get_id()) + "\"\n";
        save_file << "\"previous_direction_x\" :" << "\"" << std::to_string(pl->get_previous_direction().first) << "\"\n";
        save_file << "\"previous_direction_y\" :" << "\"" << std::to_string(pl->get_previous_direction().second) << "\"\n";
        save_file << "\"x\" :" << "\"" + std::to_string(pl->get_x()) + "\"\n";
        save_file << "\"y\" :" << "\"" + std::to_string(pl->get_y()) + "\"\n";

        save_file << "\"characteristics\" :" << "{\n";
        for (const auto& j : pl->get_characteristics())
            save_file << "\t\"" << j.first << "\":" << "\"" << j.second << "\"\n";
        save_file << "}\n";

        save_file << "\"weaponary\" :" << "[\n"; // weaponary save begin (result : array of objects)
        for (const auto& j : *pl->get_weaponary())
        {
            if(!j->get_equiped())
            {
                save_file << "\t\"" << j->get_name() << "\"" << ": {\n";
                save_file << "\t" << "\"name\":\"" << j->get_name() << "\"\n";
                save_file << "\t" << "\"class\":\"" << j->get_class() << "\"\n";
                save_file << "\t" << "\"type\":\"" << j->get_type() << "\"\n";
                save_file << "\t" << "\"id\":\"" << j->get_id() << "\"\n";
                save_file << "\t" << "\"is_equipped\": \"" << j->get_equiped() << "\"\n";
                save_file << "\t" << "\"item_characteristics\": {\n";
                for (const auto& c : *j->get_item_characteristics())
                    save_file << "\t\t\"" << c.first << "\":" << "\"" << c.second << "\"\n";
                save_file << "}\n";
                save_file << "}\n";
            }

        }
        save_file << "\t]\n";

        save_file << "\"equiped_weaponary\" :" << "[\n"; // equiped_weaponary save begin (result : array of objects)
        for (const auto& j : *pl->get_equiped_weaponary())
        {
            if(j.second != nullptr)
            {
                save_file << "\t\"" << j.second->get_name() << "\"" << ": {\n";
                save_file << "\t" << "\"slot\":\"" << j.first << "\"\n";
                save_file << "\t" << "\"name\":\"" << j.second->get_name() << "\"\n";
                save_file << "\t" << "\"class\":\"" << j.second->get_class() << "\"\n";
                save_file << "\t" << "\"type\":\"" << j.second->get_type() << "\"\n";
                save_file << "\t" << "\"id\":\"" << j.second->get_id() << "\"\n";
                save_file << "\t" << "\"is_equipped\": \"" << j.second->get_equiped() << "\"\n";
                save_file << "\t" << "\"item_characteristics\": {\n";
                for (const auto& c : *j.second->get_item_characteristics())
                    save_file << "\t\t\"" << c.first << "\":" << "\"" << c.second << "\"\n";
                save_file << "}\n";
                save_file << "}\n";
            }
        }
        save_file << "\t]\n";

        save_file << "\"armourment\" :" << "[\n"; // armoury save begin (result : array of objects)
        for (const auto& j : *pl->get_armourment())
        {
            if (!j->get_equiped())
            {
                save_file << "\t\"" << j->get_name() << "\"" << ": {\n";
                save_file << "\t" << "\"name\":\"" << j->get_name() << "\"\n";
                save_file << "\t" << "\"class\":\"" << j->get_class() << "\"\n";
                save_file << "\t" << "\"type\":\"" << j->get_type() << "\"\n";
                save_file << "\t" << "\"id\":\"" << j->get_id() << "\"\n";
                save_file << "\t" << "\"is_equipped\": \"" << j->get_equiped() << "\"\n";
                save_file << "\t" << "\"item_characteristics\": {\n";
                for (const auto& c : *j->get_item_characteristics())
                    save_file << "\t\t\"" << c.first << "\":" << "\"" << c.second << "\"\n";
                save_file << "}\n";
                save_file << "}\n";
            }
        }
        save_file << "\t]\n";

        save_file << "\"equiped_armoury\" :" << "[\n"; // equiped_armoury save begin (result : array of objects)
        for (const auto& j : *pl->get_equiped_armourment())
        {
            if(j.second != nullptr)
            {
                save_file << "\t\"" << j.second->get_name() << "\"" << ": {\n";
                save_file << "\t" << "\"slot\":\"" << j.first << "\"\n";
                save_file << "\t" << "\"name\":\"" << j.second->get_name() << "\"\n";
                save_file << "\t" << "\"class\":\"" << j.second->get_class() << "\"\n";
                save_file << "\t" << "\"type\":\"" << j.second->get_type() << "\"\n";
                save_file << "\t" << "\"id\":\"" << j.second->get_id() << "\"\n";
                save_file << "\t" << "\"is_equipped\": \"" << j.second->get_equiped() << "\"\n";
                save_file << "\t" << "\"item_characteristics\": {\n";
                for (const auto& c : *j.second->get_item_characteristics())
                    save_file << "\t\t\"" << c.first << "\":" << "\"" << c.second << "\"\n";
                save_file << "}\n";
                save_file << "}\n";
            }
        }
        save_file << "\t]\n";

        save_file << "\"jewellery\" :" << "[\n"; // jewellery save begin (result : array of objects)
        for (const auto& j : *pl->get_jewellery())
        {
            if (!j->get_equiped())
            {
                save_file << "\t\"" << j->get_name() << "\"" << ": {\n";
                save_file << "\t" << "\"name\":\"" << j->get_name() << "\"\n";
                save_file << "\t" << "\"class\":\"" << j->get_class() << "\"\n";
                save_file << "\t" << "\"type\":\"" << j->get_type() << "\"\n";
                save_file << "\t" << "\"id\":\"" << j->get_id() << "\"\n";
                save_file << "\t" << "\"is_equipped\": \"" << j->get_equiped() << "\"\n";
                save_file << "\t" << "\"item_characteristics\": {\n";
                for (const auto& c : *j->get_item_characteristics())
                    save_file << "\t\t\"" << c.first << "\":" << "\"" << c.second << "\"\n";
                save_file << "}\n";
                save_file << "}\n";
            }
        }
        save_file << "\t]\n";

        save_file << "\"equiped_jewellery\" :" << "[\n"; // equiped_jewellery save begin (result : array of objects)
        for (const auto& j : *pl->get_equiped_jewellery())
        {
            if(j.second != nullptr)
            {
                save_file << "\t\"" << j.second->get_name() << "\"" << ": {\n";
                save_file << "\t" << "\"slot\":\"" << j.first << "\"\n";
                save_file << "\t" << "\"name\":\"" << j.second->get_name() << "\"\n";
                save_file << "\t" << "\"class\":\"" << j.second->get_class() << "\"\n";
                save_file << "\t" << "\"type\":\"" << j.second->get_type() << "\"\n";
                save_file << "\t" << "\"id\":\"" << j.second->get_id() << "\"\n";
                save_file << "\t" << "\"is_equipped\": \"" << j.second->get_equiped() << "\"\n";
                save_file << "\t" << "\"item_characteristics\": {\n";
                for (const auto& c : *j.second->get_item_characteristics())
                    save_file << "\t\t\"" << c.first << "\":" << "\"" << c.second << "\"\n";
                save_file << "}\n";
                save_file << "}\n";
            }
        }
        save_file << "\t]\n";

        save_file << "\"potions\" :" << "[\n";  // potions save begin (result : array of objects)
        for (const auto& j : *pl->get_potions())
        {
            save_file << "\t\"" << j->get_name() << "\"" << ": {\n";
            save_file << "\t" << "\"name\": \"" << j->get_name() << "\"\n";
            save_file << "\t" << "\"class\": \"" << j->get_class() << "\"\n";
            save_file << "\t" << "\"type\": \"" << j->get_type() << "\"\n";
            save_file << "\t" << "\"id\": \"" << j->get_id() << "\"\n";
            save_file << "\t" << "\"is_equipped\": \"" << j->get_equiped() << "\"\n";
            save_file << "\t" << "\"effect_name\": \"" << dynamic_cast<Potion*>(j)->get_effect_name() << "\"\n";
            save_file << "\t" << "\"effect_duration\": \"" << std::to_string(dynamic_cast<Potion*>(j)->get_duration()) << "\"\n";
            save_file << "\t" << "\"item_characteristics\": {\n";
            for (const auto& c : *j->get_item_characteristics())
                save_file << "\t\t\"" << c.first << "\":" << "\"" << c.second << "\"\n";
            save_file << "}\n";
            save_file << "}\n";
        }
        save_file << "\t]\n";

        save_file << "\"active_effects\" :" << "[\n";  // effects save begin (result : array of objects)
        for (const auto& j : *pl->get_active_effects())
        {
            save_file << "\t\"" << j->get_effect_name() << "\"" << ": {\n";

            save_file << "\t" << "\"effect_name\": \"" << j->get_effect_name() << "\"\n";
            save_file << "\t" << "\"effect_duration\": \"" << std::to_string(j->get_effect_duration()) << "\"\n";
            save_file << "\t" << "\"effect_counter\": \"" << std::to_string(j->get_effect_counter()) << "\"\n";

            save_file << "}\n";

        }
        save_file << "\t]\n";

        save_file << "}\n";
    }
    save_file << "]\n";
    if(player == nullptr)
        save_file << "\"turn\" : { }\n";
    else

    {
        save_file << "\"turn\" : {\n";

        save_file << "\t\"player\" :" << "\"" + std::to_string(player->get_id()) + "\"\n";
        save_file << "\t\"game_over\" :" << "\"" + std::to_string(game_over) + "\"\n";
        save_file << "\t\"roll\" :" << "\"" + std::to_string(roll) + "\"\n";
        save_file << "\t\"is_moving\" :" << "\"" + std::to_string(is_moving) + "\"\n";
        save_file << "\t\"has_already_moved\" :" << "\"" + std::to_string(has_already_moved) + "\"\n";
        save_file << "\t\"has_attacked\" :" << "\"" + std::to_string(has_attacked) + "\"\n";
        save_file << "\t\"turn_number\" :" << "\"" + std::to_string(turn_number) + "\"\n";
        if(picked_item)
            save_file << "\t\"picked_item\" :" << "\"" + picked_item->get_name() + "\"\n";
        if(activated_event)
            save_file << "\t\"activated_event\" :" << "\"" + activated_event->get_event_name() + "\"\n";
        save_file << "\t\"start_x\" :" << "\"" + std::to_string(start_x) + "\"\n";
        save_file << "\t\"start_y\" :" << "\"" + std::to_string(start_y) + "\"\n";
        save_file << "\t\"chosen_x\" :" << "\"" + std::to_string(chosen_direction.first) + "\"\n";
        save_file << "\t\"chosen_y\" :" << "\"" + std::to_string(chosen_direction.second) + "\"\n";
        save_file << "\t\"event_finished\" :" << "\"" + std::to_string(event_finished) + "\"\n";

        save_file << "}\n";
    }

    save_file << "\"map\" : [\n";

    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
        {
            save_file << "\"" << std::to_string(i) + "und" +  std::to_string(j) << "\":";
            MapCell* tmp = &map[i][j];
            save_file << "{\n";

            save_file << "\t\"players_on_cell\" : [\n";

            for(const auto& k : tmp->get_players_on_cell())
            {
                save_file << "\t\"" << std::to_string(k->get_id()) << "\"\n";
            }

            save_file << "\t]\n";

            save_file << "\t\"i\" :" << "\"" + std::to_string(i) + "\"\n";
            save_file << "\t\"j\" :" << "\"" + std::to_string(j) + "\"\n";
            save_file << "\t\"type_of_terrain\" :" << "\"" + tmp->get_type_of_terrain() + "\"\n";
            save_file << "\t\"event_name\" :" << "\"" + tmp->get_event_name() + "\"\n";
            save_file << "\t\"item\" :" << "\"" + tmp->get_item() + "\"\n";

            save_file << "}\n";
        }

    save_file << "]\n";




    save_file << "}\n";
    save_file.close();
}

void Turn::load(std::string file_name)
{
    DataBase::get_DataBase()->clear_all_data();

    std::string save_data;
    std::ifstream save_stream(file_name);
    std::string line;
    while (save_stream.good())
    {
        std::getline(save_stream, line);
        save_data += line + "\n";
    }
    save_stream.close();

    JSONObject data(save_data);

    DataBase* current = DataBase::get_DataBase();
    current->set_height(stoi(data.get_value("height")));
    current->set_width(stoi(data.get_value("width")));

    std::string player_load_result = load_players(data.get_objects("sequence"));

    std::string map_load_result = load_map(data.get_objects("map"));

    std::string turn_load_result = load_turn(data.get_object("turn"));

}

std::string Turn::load_players(std::vector<JSONObject *> *sequence)
{
    for (const auto& i : *sequence)
    {

        Player::set_current_id(stoi(i->get_value("player_id")) - 1);
        Player* pl = new Player(i->get_value("name"));
        pl->set_previous_direction(std::make_pair(std::stoi(i->get_value("previous_direction_x")),std::stoi(i->get_value("previous_direction_y"))));
        pl->set_x(stoi(i->get_value("x")));
        pl->set_y(stoi(i->get_value("y")));

        for (const auto& ch : *i->get_object("characteristics")->get_name_to_value()) // loading characteristics
            pl->get_characteristics().insert(std::make_pair(ch.first, stoi(ch.second)));

        if(i->is_in_object_arrays("weaponary"))
        {
            for (const auto& j : *i->get_objects("weaponary")) // loading all weapons (except for the equipped ones)
            {
                JSONObject* item = j;
                int item_id = stoi(item->get_value("id"));
                std::string name = item->get_value("name");
                std::string equipment_class = item->get_value("class");
                std::string type = item->get_value("type");
                std::map<std::string, int>  item_characteristics;
                for (const auto& k : *item->get_object("item_characteristics")->get_name_to_value())
                    item_characteristics.emplace(std::make_pair(k.first, stoi(k.second)));
                Weapon* weapon = new Weapon(item_id, name, equipment_class, item_characteristics, type);
                pl->get_weaponary()->emplace(weapon);
            }
        }

        if (i->is_in_object_arrays("equiped_weaponary"))
        {
            for (const auto& j : *i->get_objects("equiped_weaponary")) // loading all equipped weaponary and adding them to the weaponary
            {
                JSONObject* item = j;
                int item_id = stoi(item->get_value("id"));
                std::string slot = item->get_value("slot");
                std::string name = item->get_value("name");
                std::string equipment_class = item->get_value("class");
                std::string type = item->get_value("type");
                std::map<std::string, int>  item_characteristics;
                for (const auto& k : *item->get_object("item_characteristics")->get_name_to_value())
                    item_characteristics.emplace(std::make_pair(k.first, stoi(k.second)));
                Weapon* weapon = new Weapon(item_id, name, equipment_class, item_characteristics, type);
                weapon->change_equiped();
                pl->get_weaponary()->emplace(weapon);
                pl->get_equiped_weaponary()->at(slot) = weapon;
            }
        }

        if (i->is_in_object_arrays("armourment"))
        {
            for (const auto& j : *i->get_objects("armourment")) // loading all armourment (except for the equipped ones)
            {
                JSONObject* item = j;
                int item_id = stoi(item->get_value("id"));
                std::string name = item->get_value("name");
                std::string equipment_class = item->get_value("class");
                std::string type = item->get_value("type");
                std::map<std::string, int>  item_characteristics;
                for (const auto& k : *item->get_object("item_characteristics")->get_name_to_value())
                    item_characteristics.emplace(std::make_pair(k.first, stoi(k.second)));
                Armour* armour = new Armour(item_id, name, equipment_class, item_characteristics, type);
                pl->get_armourment()->emplace(armour);
            }
        }

        if (i->is_in_object_arrays("equiped_armourment"))
        {
            for (const auto& j : *i->get_objects("equiped_armourment")) // loading all equipped armour and adding them to the armourment
            {
                JSONObject* item = j;
                int item_id = stoi(item->get_value("id"));
                std::string slot = item->get_value("slot");
                std::string name = item->get_value("name");
                std::string equipment_class = item->get_value("class");
                std::string type = item->get_value("type");
                std::map<std::string, int>  item_characteristics;
                for (const auto& k : *item->get_object("item_characteristics")->get_name_to_value())
                    item_characteristics.emplace(std::make_pair(k.first, stoi(k.second)));
                Armour* armour = new Armour(item_id, name, equipment_class, item_characteristics, type);
                armour->change_equiped();
                pl->get_armourment()->emplace(armour);
                pl->get_equiped_armourment()->at(slot) = armour;
            }
        }

        if (i->is_in_object_arrays("jewellery"))
        {
            for (const auto& j : *i->get_objects("jewellery")) // loading all jewellery (except for the equipped ones)
            {
                JSONObject* item = j;
                int item_id = stoi(item->get_value("id"));
                std::string name = item->get_value("name");
                std::string equipment_class = item->get_value("class");
                std::string type = item->get_value("type");
                std::map<std::string, int>  item_characteristics;
                for (const auto& k : *item->get_object("item_characteristics")->get_name_to_value())
                    item_characteristics.emplace(std::make_pair(k.first, stoi(k.second)));
                Jewel* jewel = new Jewel(item_id, name, equipment_class, item_characteristics, type);
                pl->get_jewellery()->emplace(jewel);
            }
        }

        if (i->is_in_object_arrays("equiped_jewellery"))
        {
            for (const auto& j : *i->get_objects("equiped_jewellery")) // loading all equipped jewellery and adding to jewellery
            {
                JSONObject* item = j;
                int item_id = stoi(item->get_value("id"));
                std::string slot = item->get_value("slot");
                std::string name = item->get_value("name");
                std::string equipment_class = item->get_value("class");
                std::string type = item->get_value("type");
                std::map<std::string, int>  item_characteristics;
                for (const auto& k : *item->get_object("item_characteristics")->get_name_to_value())
                    item_characteristics.emplace(std::make_pair(k.first, stoi(k.second)));
                Jewel* jewel = new Jewel(item_id, name, equipment_class, item_characteristics, type);
                jewel->change_equiped();
                pl->get_jewellery()->emplace(jewel);
                pl->get_equiped_jewellery()->at(slot) = jewel;
            }
        }

        if (i->is_in_object_arrays("potions"))
        {
            for (const auto& j : *i->get_objects("potions")) // loading all potions
            {
                JSONObject* item = j;
                int item_id = stoi(item->get_value("id"));
                int effect_duration = stoi(item->get_value("effect_duration"));
                std::string name = item->get_value("name");
                std::string equipment_class = item->get_value("class");
                std::string type = item->get_value("type");
                std::string effect_name = item->get_value("effect_name");
                std::map<std::string, int>  item_characteristics;
                for (const auto& k : *item->get_object("item_characteristics")->get_name_to_value())
                    item_characteristics.emplace(std::make_pair(k.first, stoi(k.second)));
                Potion* potion = new Potion(item_id, name, equipment_class, item_characteristics, type, effect_duration, effect_name);
                pl->get_potions()->emplace(potion);
            }
        }

        if (i->is_in_object_arrays("active_effects"))
        {
            for (const auto& j : *i->get_objects("active_effects")) // loading all potions
            {
                JSONObject* item = j;
                std::string effect_name = item->get_value("effect_name");
                int effect_duration = stoi(item->get_value("effect_duration"));
                int effect_counter = stoi(item->get_value("effect_counter"));
                All_effects::get_effects_data()->get_effects()->at(effect_name)->apply_effect(*pl, effect_duration, effect_counter);
            }
        }

        DataBase::get_DataBase()->get_sequence()->push_back(pl);
    }
    return "success";
}

std::string Turn::load_map(std::vector<JSONObject *> *map)
{
    MapCell** karta = new MapCell* [DataBase::get_DataBase()->get_height()];
    for (int i = 0; i < DataBase::get_DataBase()->get_height(); i++)
        karta[i] = new MapCell[DataBase::get_DataBase()->get_width()];

    for (const auto& i : *map)
    {

        std::string type_of_terrain = i->get_value("type_of_terrain");
        std::string event_name = i->get_value("event_name");
        std::string item = i->get_value("item");
        MapCell tmp(type_of_terrain, item, event_name);
        if(i->is_in_arrays("players_on_cell"))
            for (const auto& j : *i->get_values("players_on_cell"))
            {
                Player* ptr = nullptr;
                int id = stoi(j);
                for(const auto& k : *DataBase::get_DataBase()->get_sequence())
                    if(k->get_id() ==  id)
                        ptr = k;
                if(ptr == nullptr)
                    return "Игрок с ID == " + j + " не был найден";
                tmp.get_players_on_cell().insert(ptr);
            }
        int I = std::stoi(i->get_value("i"));
        int J = std::stoi(i->get_value("j"));
        karta[I][J] = tmp;
    }
    DataBase::get_DataBase()->set_map(karta);
    return "success";
}

std::string Turn::load_turn(JSONObject *turn_object)
{
    has_already_moved = (std::stoi(turn_object->get_value("has_already_moved")));
    chosen_direction = std::make_pair(std::stoi(turn_object->get_value("chosen_x")), std::stoi(turn_object->get_value("chosen_y")));
    event_finished = (std::stoi(turn_object->get_value("event_finished")));
    has_attacked = (std::stoi(turn_object->get_value("has_attacked")));
    int id = std::stoi(turn_object->get_value("player"));
    for(const auto& i : *DataBase::get_DataBase()->get_sequence())
        if(i->get_id() == id)
            player = i;
    game_over = (std::stoi(turn_object->get_value("game_over")));
    roll = (std::stoi(turn_object->get_value("roll")));
    is_moving = (std::stoi(turn_object->get_value("is_moving")));

    start_x = (std::stoi(turn_object->get_value("start_x")));
    start_y = (std::stoi(turn_object->get_value("start_y")));
    turn_number = (std::stoi(turn_object->get_value("turn_number")));
    return "success";
}

void Turn::change_player_position(const int& x1, const int& y1, const int& x2, const int& y2) const
{
    MapCell **map = DataBase::get_DataBase()->get_map();
    map[y1][x1].pop_player(player);
    map[y2][x2].add_player(player);
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
    change_player_position(start_x, start_y, player->get_x(), player->get_y());
    MapCell **map = DataBase::get_DataBase()->get_map();

    if(map[player->get_y()][player->get_x()].get_item() != "Нет")
    {
        picked_item = player->add_item(map[player->get_y()][player->get_x()].get_item());
        map[player->get_y()][player->get_x()].set_item("Нет");
    }
    if(map[player->get_y()][player->get_x()].get_event_name()!="???")
        activated_event = Events::get_Events()->get_events()->at("mushrooms");
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
    save("../Game/Saves/SAVE_FILE.txt");
};
