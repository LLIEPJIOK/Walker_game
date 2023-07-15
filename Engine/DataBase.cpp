#include "DataBase.h"
DataBase* DataBase::data = 0;

DataBase::DataBase()
{
    std::ifstream fin;
    fin.open("../Game/Resources/Files/all_equipment_data.txt");
	if (!fin.is_open())
        throw;
	fin.peek();
	if (!fin.good())
        throw;

	std::string line;
	std::string all_equip;
	while (fin.good())
	{
		getline(fin, line);
		all_equip += line + '\n';
    }
    fin.close();
    all_equipment = new JSONObject(all_equip);
    fin.open("../Game/Resources/Files/equipment_list.txt");
    while(fin.good())
    {
        getline(fin, line);
        equipment_list.push_back(line);
    }
    fin.close();


    fin.open("../Game/Resources/Files/jewellery_stats.txt");
    std::string text = "";
    while(fin.good())
    {
        getline(fin, line);
        text+=line;
    }
    jewellery_stats = new JSONObject(text);
    fin.close();
}

void DataBase::generate_items()
{
    std::srand(time(NULL));
    int x, y;
    for(int i = 0; i < 200; i++)
    {
        do
        {
            x = std::rand()%width;
            y = std::rand()%height;
        }while(map[y][x].get_item() != "Нет" || map[y][x].get_type_of_terrain() == "non_moving_area" );

        map[y][x].set_item(equipment_list[std::rand()%equipment_list.size()]);
    }
}

void DataBase::save(std::ofstream &out)
{
    //запись height
    out.write((char*)& height, sizeof(height));
    //запись width
    out.write((char*)& width, sizeof(width));
    //внешний цикл для записи каждого объекта класса MapCell
    for(int i = 0; i < height; i++)
    {
        //внутренний цикл для записи каждого объекта класса MapCell
        for(int j = 0; j < width; j++)
        {
            //вызов метода записи объекта класса MapCell
            map[i][j].save(out);
        }
    }

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
}

void DataBase::load(std::ifstream &in)
{
    //чтение height
    in.read((char*)& height, sizeof(height));
    //чтение width
    in.read((char*)& width, sizeof(width));

    //создание массива укаателей на MapCell рамзером  height
    map = new MapCell*[height];
    //внешний цикл для чтения каждого объекта класса MapCell
    for(int i = 0; i < height; i++)
    {
        //создание массива объектов класса MapCell
        map[i] = new MapCell[width];
        //внутрениий цикл для чтения каждого объекта класса MapCell
        for(int j = 0; j < width; j++)
        {
            //вызов метода чтения объекта класса MapCell
            map[i][j].load(in);
        }
    }

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
}

JSONObject* DataBase::get_all_equipment_data()
{
    return all_equipment;
}

JSONObject *DataBase::get_jewellery_stats()
{
    return  jewellery_stats;
}

std::vector<Player*>* DataBase::get_sequence()
{
    return &sequence;
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
    }
}

DataBase::~DataBase()
{
    for (int i = 0; i < height; ++i)
        delete[]map[i];
    delete[]map;
	for (auto i = sequence.begin(); i != sequence.end(); ++i)
        delete *i;
    delete all_equipment;
    delete jewellery_stats;
    data = 0;
}

DataBase* DataBase::get_DataBase()
{
	if (!data)
        data = new DataBase();
    return data;
}

MapCell** DataBase::get_map() const
{
    return map;
}

int DataBase::get_height() const
{
    return height;
}

int DataBase::get_width() const
{
    return width;
}

void DataBase::set_height(int _height)
{
    height = _height;
}

void DataBase::set_width(int _width)
{
    width = _width;
}

void DataBase::generate_map()
{
    std::ifstream fin(map_file);
    if (!fin.is_open())
        throw;
    fin.peek();
    if (!fin.good())
        throw;
    fin >> height >> width;

    map = new MapCell* [height];
    int c;
    for (int i = 0; i < height; i++)
    {
        map[i] = new MapCell[width];
        for (int j = 0; j < width; j++)
        {
            fin >> c;
            if (c)
                map[i][j].set_type_of_terrain("moving_area");
            map[i][j].set_event_name("experiment");
        }
    }
    fin.close();

    for(int i = 0; i < height/2; i++)
        for(int j = 0; j < width/2; j++)
        {
            if(map[i][j].get_type_of_terrain() == "moving_area")
            {
                map[i][j].set_type_of_terrain("Лес");
                map[i][j].set_tile_name("forest_");
            }
            else
                map[i][j].set_tile_name("forest_1");
            if(map[height-1-i][j].get_type_of_terrain() == "moving_area")
            {
                map[height-1-i][j].set_type_of_terrain("Пустыня");
                map[height-1-i][j].set_tile_name("desert_");
            }
            else
                map[height-1-i][j].set_tile_name("desert_1");
        }

    for(int i = 0; i < height/2; i++)
        for(int j = width/2; j < width; j++)
        {
            if(map[i][j].get_type_of_terrain() == "moving_area")
            {
                map[i][j].set_type_of_terrain("Горы");
                map[i][j].set_tile_name("tundra_");
            }
            else
                map[i][j].set_tile_name("tundra_1");
            if(map[height-1-i][j].get_type_of_terrain() == "moving_area")
            {
                map[height-1-i][j].set_type_of_terrain("Болото");
                map[height-1-i][j].set_tile_name("swamp_");
            }
            else
                map[height-1-i][j].set_tile_name("swamp_1");
        }
}

void DataBase::set_map(MapCell**& map)
{
    this->map = map;
}
