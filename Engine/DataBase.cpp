#include "DataBase.h"
#include <fstream>
#include <exception>

DataBase* DataBase::data = 0;

using json = nlohmann::json;

DataBase::DataBase()
{
    initialize_json(all_equipment, "../Game/Resources/Files/all_equipment_data.txt");
    initialize_json(all_effects, "../Game/Resources/Files/all_effects_data.txt");
    initialize_json(jewellery_stats, "../Game/Resources/Files/jewellery_stats.txt");

    // equipment_list.txt нужно будет переделать под JSON, когда будем реализовывать тиры предметов
    std::ifstream fin;
    fin.open("../Game/Resources/Files/equipment_list.txt");
    if (!fin.is_open())
        throw std::exception("equipment_list.txt failed to open");
    fin.peek();
    if (!fin.good())
        throw std::exception("equipment_list.txt is empty");

    std::string line = "";

    while(fin.good())
    {
        getline(fin, line);
        equipment_list.push_back(line);
    }

    fin.close();

    map = GraphMap(50, 50);
}

void DataBase::initialize_json(nlohmann::json &field, const std::string &path)
{
    std::ifstream fin;
    fin.open(path);
    field = json::parse(fin);
    fin.close();
}

nlohmann::json DataBase::get_all_equipment_data() const
{
    return all_equipment;
}

nlohmann::json DataBase::get_jewellery_stats() const
{
    return  jewellery_stats;
}

nlohmann::json DataBase::get_all_effects_data() const
{
    return all_effects;
}

std::vector<Player*>* DataBase::get_sequence()
{
    return &sequence;
}

GraphMap *DataBase::get_graph_map()
{
    return &map;
}

void DataBase::generate_players(std::vector<std::pair<std::string, std::string> > data)
{
    for(int i = 0; i < data.size(); i++)
    {
        Player *player = new Player(data[i].first);
        sequence.push_back(player);
        sequence[i]->get_characteristics()["STR"] = data[i].second[0] - 48;
        sequence[i]->get_characteristics()["AGIL"] = data[i].second[1] - 48;
        sequence[i]->get_characteristics()["INT"] = data[i].second[2] - 48;
        player->update_chars();
    }
}

void DataBase::generate_events()
{

}

void DataBase::generate_items()
{
    srand((unsigned int)time(NULL));
    for(auto& i : map.getMap()) {
        i.second.set_item(equipment_list[rand() % equipment_list.size()]);
    }
}

DataBase::~DataBase()
{
	for (auto i = sequence.begin(); i != sequence.end(); ++i)
        delete *i;

    data = 0;
}

DataBase* DataBase::get_DataBase()
{
	if (!data)
        data = new DataBase();
    return data;
}



void DataBase::save(QFile &out)
{
    //размер контейнера sequence
    size_t size = sequence.size();
    //запись размера sequence
    out.write((char*)& size, sizeof(size));
    //цикл для записи каждого объекта класса Player
    for(auto& i: sequence)
    {
        //вызов метода записи объекта класса Player
        i->save(out);
    }

    map.save(out);
}

void DataBase::load(QFile &in)
{
    //переменная для размера контейнера
    size_t size;
    //чтение размера контейнера sequence
    in.read((char*)& size, sizeof(size));
    //очистка контейнера sequence
    sequence.clear();
    sequence.reserve(size);
    //цикл для заполнения контейнера данными
    for(int i = 0; i < size; i++)
    {
        //создание нового объекта класса Player
        Player* pl = new Player("");
        //вызов метода чтения объекта класса Player
        pl->load(in);
        //добавление в контейнер указателя на объект класса Player
        sequence.push_back(pl);
    }

    map.load(in);
}

