#ifndef DATABASE_H
#define DATABASE_H

#include <fstream>
#include <exception>
#include <map>
#include <iostream>
#include "Json.h"
#include "mapcell.h"

const std::string map_file = "../Game/Resources/Files/in.txt";

class DataBase
{
    MapCell** map;
    int height, width;
    std::vector <std::string> equipment_list;
	JSONObject* all_equipment;
    JSONObject* jewellery_stats;

    std::vector <Player*> sequence;
	static DataBase* data;

	const DataBase& operator = (const DataBase&) = delete;
	DataBase(const DataBase&) = delete;

    DataBase();
public:
	~DataBase();
    static DataBase *get_DataBase();
    MapCell** get_map() const;
    int get_height() const;
    int get_width() const;

    void set_height(int _height);
    void set_width(int _width);


    void set_map(MapCell**& map);
    JSONObject* get_all_equipment_data();
    JSONObject* get_jewellery_stats();
    std::vector <Player*>* get_sequence();

    void generate_map();
    void generate_players(std::vector<std::pair<std::string, std::string>> data);
    void generate_items();

    void save(std::ofstream& out);
    void load(std::ifstream& in);
};

#endif //DATABASE_H
