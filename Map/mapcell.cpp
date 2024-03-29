#include "mapcell.h"
#include "Events/Event.h"

MapCell::MapCell()
{
    this->type_of_terrain = Wall;
    this->item = "None";
    int ev = rand() % Events::get_Events()->get_events()->size(); //рандомный номер ивента из всех
    std::map<std::string, Event*>::iterator it = Events::get_Events()->get_events()->begin();
    std::advance(it, ev); // адванс до нужного
    this->event_name = it->first;
}

MapCell::MapCell(LocationType type_of_terrain, std::string item, std::string event_name)
{
    this->type_of_terrain = type_of_terrain;
    this->item = item;
    this->event_name = event_name;
}

LocationType MapCell::get_type_of_terrain() const
{
    return type_of_terrain;
}

std::string MapCell::get_item() const
{
    return item;
}

std::string MapCell::get_event_name() const
{
    return event_name;
}

std::string MapCell::get_tile_name() const
{
    return tile_name;
}

void MapCell::set_type_of_terrain(LocationType type_of_terrain)
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

void MapCell::save(QFile &out)
{
    //запись type_of_terrain
    out.write((char*)& type_of_terrain, sizeof(type_of_terrain));

    //размер item c /0
    size_t size = item.size() + 1;
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

void MapCell::load(QFile &in)
{
    //переменная для размера строк
    size_t size;
    //чтение type_of_terrain
    in.read((char*)&type_of_terrain, sizeof(type_of_terrain));

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
