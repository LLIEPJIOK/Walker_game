#ifndef DATABASE_H
#define DATABASE_H
#include <QFile>
#include "Player.h"
#include "Map/GraphMap.h"

#include "Json/json.hpp"

const std::string map_file = "../Game/Resources/Files/in.txt";

class DataBase
{

    GraphMap map;

    std::vector <std::string> equipment_list;

    nlohmann::json all_equipment;
    nlohmann::json all_effects;
    nlohmann::json jewellery_stats;

    std::vector <Player*> sequence;
	static DataBase* data;

	const DataBase& operator = (const DataBase&) = delete;
	DataBase(const DataBase&) = delete;

    DataBase();

    void initialize_json(nlohmann::json& field, const std::string& path);
public:
	~DataBase();
    static DataBase *get_DataBase();
    nlohmann::json get_all_equipment_data() const;
    nlohmann::json get_jewellery_stats() const;
    nlohmann::json get_all_effects_data() const;
    std::vector <Player*>* get_sequence();

    GraphMap* get_graph_map();

    void generate_players(std::vector<std::pair<std::string, std::string>> data);
    void generate_events();
    void generate_items();

    void save(QFile &out);
    void load(QFile &in);
};

#endif //DATABASE_H
