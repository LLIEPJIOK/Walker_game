
#ifndef MAPCELL_H
#define MAPCELL_H


#include "Player.h"

class MapCell
{
private:
    std::string type_of_terrain;
    std::string item;
    std::string event_name;
    std::string tile_name;
public:
    MapCell();
    MapCell(std::string type_of_terrain, std::string item, std::string event_name);

    std::string get_type_of_terrain();
    std::string get_item();
    std::string get_event_name();
    std::string get_tile_name();


    void set_type_of_terrain(std::string type_of_terrain);
    void set_item(std::string item);
    void set_event_name(std::string event_name);
    void set_tile_name(std::string tile_name);

    void initiate_event();

    void save(std::ofstream& out);
    void load(std::ifstream& in);
};

#endif // MAPCELL_H
