#include "DataBase.h"

Equipment::Equipment(std::string name)
{
    nlohmann::json js = DataBase::get_DataBase()->get_all_equipment_data()[name];
    item_id = js["ID"];
    this->name = name;
    auto ch_p = js[name];
    if (DataBase::get_DataBase()->get_all_equipment_data()[name].contains("chars")) {
        nlohmann::json k = DataBase::get_DataBase()->get_all_equipment_data().at(name).at("chars");
        for (auto& [key, value] : k.items()) {
            item_characteristics.emplace(std::make_pair(key, value));
        }
    }
//    for (auto& ch_p : js[name]["chars"].items()){
//        item_characteristics[ch_p.key()] = ch_p.value();
//    }
    type = js["type"];
    equipment_class = js["class"];
    is_equiped = false;
    is_front_equiped = false;
    primary_slot = false;

}

Equipment::Equipment(int ID, std::string _name, std::string _equipment_class, std::unordered_map<std::string, int> characteristics, std::string _type)
{
    item_id = ID;
    name = _name;
    item_characteristics = characteristics;
    type = _type;
    equipment_class = _equipment_class;
    is_equiped = false;
    is_front_equiped = false;
    primary_slot = false;
}

std::string Equipment::get_name() const
{
    return name;
}

const std::unordered_map<std::string, int>* Equipment::get_item_characteristics() const
{
    return &item_characteristics;
}

int Equipment::get_id() const
{
    return item_id;
}

bool Equipment::get_equiped() const
{
    return is_equiped;
}

bool Equipment::get_front_equiped() const
{
    return is_front_equiped;
}

bool Equipment::get_primary_equipped() const
{
    return primary_slot;
}

std::string Equipment::get_type() const
{
    return type;
}

std::string Equipment::get_class() const { return equipment_class; }

void Equipment::change_equiped()
{
    is_equiped = !is_equiped;
}

void Equipment::change_front_equiped()
{
    is_front_equiped = !is_front_equiped;
}

void Equipment::set_primary_equipped(bool val)
{
    primary_slot = val;
}

//Jewellery
void Jewel::augment(std::string tier, std::string field, std::string choice)
{
    std::string chosen_stat = DataBase::get_DataBase()->get_jewellery_stats()["choice"][field][choice];
    name += DataBase::get_DataBase()->get_jewellery_stats()["char_names"][chosen_stat];
    item_characteristics.insert(std::make_pair(chosen_stat, DataBase::get_DataBase()->get_jewellery_stats()[tier]["chars"][field][chosen_stat]));
}

Jewel::Jewel(int ID, std::string name, std::string equipment_class, std::unordered_map<std::string, int> characteristics, std::string type)
    : Equipment(ID, name, equipment_class, characteristics, type)
{

}

Jewel::Jewel(std::string type, int turn_number) : Equipment()
{
    if (turn_number < 1)
    {
        throw std::exception("turn number is less than 11");
    }


    std::default_random_engine e1(r());
     std::uniform_int_distribution<int> uniform_dist(1, 3);
    int base = uniform_dist(e1), spec = uniform_dist(e1), utility = uniform_dist(e1);
    item_id = base*100 + spec*10 + utility + 660;
    equipment_class = "jewel";
    this->type = type;
    is_equiped = 0;
    std::string tier = "T" + std::to_string(int(turn_number > 20) + int(turn_number > 50) + 1);

    name = DataBase::get_DataBase()->get_jewellery_stats()[tier]["name"][type];
    name += " of ";
    if (type == "necklace"){
        augment(tier, "special", std::to_string(spec));
        name += " and ";
        augment(tier, "util", std::to_string(utility));
    }
    else if (type == "belt")
        augment(tier, "util", std::to_string(utility));
    else
        augment(tier, "special", std::to_string(spec));

    augment(tier, "attributes", std::to_string(base));
}

//Weaponary
Weapon::Weapon(int ID, std::string name, std::string equipment_class, std::unordered_map<std::string, int> characteristics, std::string type)
    : Equipment(ID, name, equipment_class, characteristics, type)
{

}

//Armourment
Armour::Armour(int ID, std::string name, std::string equipment_class, std::unordered_map<std::string, int> characteristics, std::string type)
    : Equipment(ID, name, equipment_class, characteristics, type)
{

}

//Potion
void Potion::save(QFile &out)
{
    Equipment::save(out);
    out.write((char*)& duration, sizeof(duration));

    size_t size_of_effect_name = effect_name.size() + 1;
    out.write((char*)& size_of_effect_name, sizeof(size_of_effect_name));
    out.write(effect_name.c_str(), size_of_effect_name);
}

void Potion::load(QFile& in)
{
    Equipment::load(in);
    in.read((char*)&duration, sizeof(duration));

    size_t size;
    in.read((char*)& size, sizeof(size));
    effect_name = std::string(size - 1, ' ');
    in.read(effect_name.data(), size);
}

Potion::Potion(int ID, std::string name, std::string equipment_class, std::unordered_map<std::string, int> characteristics, std::string type, int _duration, std::string _effect_name)
    : Equipment(ID, name, equipment_class, characteristics, type)

{
    duration = _duration;
    effect_name = _effect_name;
}

Potion::Potion(std::string name) : Equipment(name)
{
    nlohmann::json js = DataBase::get_DataBase()->get_all_equipment_data()[name];
    duration = js["duration"];
    effect_name = js["effect_name"];
}

void Potion::dec_duration()
{
    duration--;
}

int Potion::get_duration() const
{
    return duration;
}

std::string Potion::get_effect_name() const { return effect_name; }

void Equipment::save(QFile& out)
{
    //запись item_id
    out.write((char*)& item_id, sizeof(item_id));

    //размер name с /0
    size_t size = name.size() + 1;
    //запись размера name
    out.write((char*)& size, sizeof(size));
    //запись name
    out.write(name.c_str(), size);

    //размер контейнера item_characteristics
    size = item_characteristics.size();
    //запись рамзера item_characteristics
    out.write((char*)& size, sizeof(size));
    //цикл для записи всего контейнера
    for(const auto &i : item_characteristics)
    {
        //размер ключа
        size_t string_size = i.first.size() + 1;
        //запись размера ключ
        out.write((char*)& string_size, sizeof(string_size));
        //запись ключа
        out.write(i.first.c_str(), string_size);

        //запись значения
        out.write((char*)& i.second, sizeof(i.second));
    }

    //размер equipment_class c /0
    size = equipment_class.size() + 1;
    //запись размера equipment_class
    out.write((char*)& size, sizeof(size));
    // запись equipment_class
    out.write(equipment_class.c_str(), size);

    //размер type с /0
    size = type.size() + 1;
    //запись размера type
    out.write((char*)& size, sizeof(size));
    //запись type
    out.write(type.c_str(), size);

    //запись is_equiped
    out.write((char*)& is_equiped, sizeof(is_equiped));

    //запись primary_slot
    out.write((char*)& primary_slot, sizeof(primary_slot));
}

void Equipment::load(QFile &in)
{
    //чтение item_id
    in.read((char*)&item_id, sizeof(item_id));

    //переменная для размера строк и контейнеров
    size_t size;

    //чтение размера name
    in.read((char*)& size, sizeof(size));
    //присваивание name строки из пробелов длиной size-1, без /0
    name = std::string(size - 1, ' ');
    //чтение name
    in.read(name.data(), size);

    //чтение размера контейнера item_characteristics
    in.read((char*)& size, sizeof(size));
    //очистка контейнера item_characteristics
    item_characteristics.clear();

    //цикл для заполнения контейнера данными
    for(int i = 0; i < size; i++)
    {
        //переменная для размера ключа
        size_t string_size;
        //чтение размера ключа
        in.read((char*)& string_size, sizeof(string_size));
        //присваивание ключу строки из пробелов длиной size-1, без /0
        std::string key(string_size - 1, ' ');
        //чтение ключа
        in.read(key.data(), string_size);

        //переменная для значения
        int value;
        //чтение занчения
        in.read((char*)& value, sizeof(value));

        //добавление или изменение пары ключ-значение
        item_characteristics[key] = value;
    }

    //чтение размера equpment_class
    in.read((char*)& size, sizeof(size));
    //присваивание equipment_class строки из пробелов длиной size-1, без /0
    equipment_class = std::string(size - 1, ' ');
    //чтение equipment_class
    in.read(equipment_class.data(), size);

    //чтение размера type
    in.read((char*)& size, sizeof(size));
    //присваивание type строки из пробелов длиной size-1, без /0
    type = std::string(size - 1, ' ');
    //чтение type
    in.read(type.data(), size);

    //чтение is_equiped
    in.read((char*)& is_equiped, sizeof(is_equiped));

    in.read((char*)& primary_slot, sizeof(primary_slot));
}



