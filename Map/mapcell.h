#ifndef MAPCELL_H
#define MAPCELL_H

#include "Engine/Player.h"

enum LocationType {
    Ground = 1,
    Grass,
    Water,
    Sand,
    Wall
};

class MapCell
{
private:
    LocationType type_of_terrain;
    std::string item;
    std::string event_name;
    std::string tile_name;
    //std::vector<Player*> players_on_cell;
public:
    MapCell();
    MapCell(LocationType type_of_terrain, std::string item, std::string event_name);

    LocationType get_type_of_terrain() const;
    std::string get_item() const;
    std::string get_event_name() const;
    std::string get_tile_name() const;

    void set_type_of_terrain(LocationType type_of_terrain);
    void set_item(std::string item);
    void set_event_name(std::string event_name);
    void set_tile_name(std::string tile_name);

    void save(QFile& out);
    void load(QFile& in);
};

#endif // MAPCELL_H
