#include "mapcell.h"

MapCell::MapCell()
{
    this->type_of_terrain = "non_moving_area";
    this->item = "Нет";
    this->event_name = "???";
}

MapCell::MapCell(std::string type_of_terrain, std::string item, std::string event_name)
{
    this->type_of_terrain = type_of_terrain;
    this->item = item;
    this->event_name = event_name;
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

void MapCell::save(std::ofstream &out)
{
    //размер type_of_terrain c /0
    size_t size = type_of_terrain.length() + 1;
    //запись размера type_of_terrain
    out.write((char*)& size, sizeof(size));
    //запись type_of_terrain
    out.write(type_of_terrain.c_str(), size);

    //размер item c /0
    size = item.size() + 1;
    //запись зазмера item
    out.write((char*)& size, sizeof(size));
    //запись item
    out.write(item.c_str(), size);

    //размер event_name c /0
    size = event_name.size() + 1;
    //запись размера event_name
    out.write((char*)& size, sizeof(size));
    //запись event_name
    out.write(event_name.c_str(), size);

    //размер tile_name с /0
    size = tile_name.size() + 1;
    //запись размера tile_name
    out.write((char*)& size, sizeof(size));
    //запись tile_name
    out.write(tile_name.c_str(), size);
}

void MapCell::load(std::ifstream &in)
{
    //переменная для размера строк
    size_t size;
    //чтения размера type_of_terrain
    in.read((char*)& size, sizeof(size));
    //присваивание type_of_terrain строки из пробелов длиной size-1, без /0
    type_of_terrain = std::string(size - 1, ' ');
    //чтение type_of_terrain
    in.read(type_of_terrain.data(), size);

    //чтение размера item
    in.read((char*)& size, sizeof(size));
    //присваивание item строки из пробелов длиной size-1, без /0
    item = std::string(size - 1, ' ');
    //чтение item
    in.read(item.data(), size);

    //чтение размера event_name
    in.read((char*)& size, sizeof(size));
    //присваивание event_name строки из пробелов длиной size-1, без /0
    event_name = std::string (size - 1, ' ');
    //чтение event_name
    in.read(event_name.data(), size);

    //чтение размера tile_name
    in.read((char*)& size, sizeof(size));
    //присваивание tile_name строки из пробелов длиной size-1, без /0
    tile_name = std::string(size - 1, ' ');
    //чтение tile_name
    in.read(tile_name.data(), size);
}
