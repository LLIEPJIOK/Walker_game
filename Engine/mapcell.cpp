
#include "mapcell.h"
#include "Events/Event.h"




MapCell::MapCell()
{
    this->type_of_terrain = "non_moving_area";
    this->item = "Нет";
    int ev = rand() % Events::get_Events()->get_events()->size(); //рандомный номер ивента из всех
    std::map<std::string, Event*>::iterator it = Events::get_Events()->get_events()->begin();
    std::advance(it, ev); // адванс до нужного
    this->event_name = it->first;
}

MapCell::MapCell(std::string type_of_terrain, std::string item, std::string event_name)
{
    this->type_of_terrain = type_of_terrain;
    this->item = item;
    this->event_name = event_name;
}

std::set<Player *> MapCell::get_players_on_cell()
{
    return players_on_cell;
}

std::string MapCell::get_type_of_terrain()
{
    return type_of_terrain;
}

std::string MapCell::get_item()
{
    return item;
}

std::string MapCell::get_event_name()
{
    return event_name;
}

std::string MapCell::get_tile_name()
{
    return tile_name;
}

void MapCell::set_type_of_terrain(std::string type_of_terrain)
{
    this->type_of_terrain = type_of_terrain;
}

void MapCell::set_item(std::string item)
{
    this->item = item;
}

void MapCell::set_event_name(std::string event_name)
{
    this->event_name = event_name;
}

void MapCell::set_tile_name(std::string tile_name)
{
    this->tile_name = tile_name;
}

void MapCell::add_player(Player *player)
{
    players_on_cell.insert(player);
}

void MapCell::pop_player(Player *player)
{
    players_on_cell.erase(player);
}
