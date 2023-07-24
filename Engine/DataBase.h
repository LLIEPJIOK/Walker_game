#ifndef DATABASE_H
#define DATABASE_H

#include "Json.h"
#include "mapcell.h"

#include <exception>
#include <map>
#include <iostream>
#include <fstream>
#include <QFile>

const std::string map_file = "../Game/Resources/Files/in.txt";

class DataBase
{
    MapCell** map;
    int height, width;
    std::vector <std::string> equipment_list;

	JSONObject* all_equipment;
    JSONObject* all_effects;
    JSONObject* jewellery_stats;

    std::vector <Player*> sequence;
	static DataBase* data;

	const DataBase& operator = (const DataBase&) = delete;
	DataBase(const DataBase&) = delete;

    DataBase();

    void initialize_equipment();
    void initialize_effects();
    void initialize_jewellery_stats();
public:
	~DataBase();
    static DataBase *get_DataBase();
    MapCell** get_map() const;
    int get_height() const;
    int get_width() const;
    JSONObject* get_all_equipment_data() const;
    JSONObject* get_jewellery_stats() const;
    JSONObject* get_all_effects_data() const;
    std::vector <Player*>* get_sequence();

    void set_height(int _height);
    void set_width(int _width);
    void set_map(MapCell**& map);

    void generate_map();
    void generate_players(std::vector<std::pair<std::string, std::string>> data);
    void generate_items();

    void save(QFile &out);
    void load(QFile &in);
};

#endif //DATABASE_H
