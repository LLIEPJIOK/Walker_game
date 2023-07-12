
#include "DataBase.h"

void Equipment::save(std::ofstream& out)
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
}

void Equipment::load(std::ifstream& in)
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
}

Equipment::Equipment(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type)
{
    item_id = ID;
    this->name = name;
    item_characteristics = characteristics;
    this->type = type;
    this->equipment_class = equipment_class;
    is_equiped = 0;
}

std::string Equipment::get_name() const { return name; }

const std::map<std::string, int>* Equipment::get_item_characteristics() const { return &item_characteristics; }

int Equipment::get_id() const { return item_id; }

bool Equipment::get_equiped() const { return is_equiped; }

std::string Equipment::get_type() const { return type; }

std::string Equipment::get_class() const { return equipment_class; }

void Equipment::change_equiped() { is_equiped = !is_equiped; }


//Jewellery
void Jewel::make_ring(int turn_number)
{
    int base = rand() % 3 + 1, spec = rand() % 3 + 1, utility = rand() % 3 + 1;
    item_id = base*100 + spec*10 + utility + 600;
    equipment_class = "украшение";
    type = "кольцо";
    is_equiped = 0; 
    if(turn_number < 20)
    {
        name = "Малое кольцо ";
        if(spec == 1)
        {
            item_characteristics.insert(std::make_pair("PIERCE",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("STR"))));
            name += "пронзания";
        }
        else if(spec == 2)
        {
            item_characteristics.insert(std::make_pair("CRIT_CH",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("CRIT_CH"))));
            name+= "шанса крит. удара";
        }
        else
        {
            item_characteristics.insert(std::make_pair("CRIT_DMG",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("CRIT_DMG"))));
            name += "критческого удара";
        }

        if(base == 1)
        {
            item_characteristics.insert(std::make_pair("STR",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("STR"))));
            name += "(STR)";
        }
        else if(base == 2)
        {
            item_characteristics.insert(std::make_pair("AGIL",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("AGIL"))));
            name+="(AGIL)";
        }
        else
        {
            item_characteristics.insert(std::make_pair("INT",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("INT"))));
            name += "(INT)";
        }
    }
    else if(turn_number > 50)
    {
        name = "Великое кольцо ";
        if(spec == 1)
        {
            item_characteristics.insert(std::make_pair("PIERCE",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("STR"))));
            name += "пронзания";
        }
        else if(spec == 2)
        {
            item_characteristics.insert(std::make_pair("CRIT_CH",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("CRIT_CH"))));
            name+= "шанса крит. удара";
        }
        else
        {
            item_characteristics.insert(std::make_pair("CRIT_DMG",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("CRIT_DMG"))));
            name += "критческого удара";
        }

        if(base == 1)
        {
            item_characteristics.insert(std::make_pair("STR",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("STR"))));
            name += "(STR)";
        }
        else if(base == 2)
        {
            item_characteristics.insert(std::make_pair("AGIL",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("AGIL"))));
            name+="(AGIL)";
        }
        else
        {
            item_characteristics.insert(std::make_pair("INT",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("INT"))));
            name += "(INT)";
        }
    }
    else
    {
        name = "Кольцо ";
        if(spec == 1)
        {
            item_characteristics.insert(std::make_pair("PIERCE",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("STR"))));
            name += "пронзания";
        }
        else if(spec == 2)
        {
            item_characteristics.insert(std::make_pair("CRIT_CH",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("CRIT_CH"))));
            name+= "шанса крит. удара";
        }
        else
        {
            item_characteristics.insert(std::make_pair("CRIT_DMG",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("CRIT_DMG"))));
            name += "критческого удара";
        }

        if(base == 1)
        {
            item_characteristics.insert(std::make_pair("STR",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("STR"))));
            name += "(STR)";
        }
        else if(base == 2)
        {
            item_characteristics.insert(std::make_pair("AGIL",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("AGIL"))));
            name+="(AGIL)";
        }
        else
        {
            item_characteristics.insert(std::make_pair("INT",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("INT"))));
            name += "(INT)";
        }
    }
}

void Jewel::make_necklace(int turn_number)
{
    int base = rand() % 3 + 1, spec = rand() % 3 + 1, utility = rand() % 3 + 1;
    item_id = base*100 + spec*10 + utility + 630;
    equipment_class = "украшение";
    type = "ожерелье";
    is_equiped = 0;
    if(turn_number < 20)
    {
        name = "Малое ожерелье ";
        if(spec == 1)
        {
            item_characteristics.insert(std::make_pair("PIERCE",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("STR"))));
            name += "пронзания ";
        }
        else if(spec == 2)
        {
            item_characteristics.insert(std::make_pair("CRIT_CH",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("CRIT_CH"))));
            name+= "шанса крит. удара ";
        }
        else
        {
            item_characteristics.insert(std::make_pair("CRIT_DMG",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("CRIT_DMG"))));
            name += "критческого удара ";
        }

        if(spec == 1)
        {
            item_characteristics.insert(std::make_pair("HP",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("HP"))));
            name += "и здоровья";
        }
        else if(spec == 2)
        {
            item_characteristics.insert(std::make_pair("CRIT_CH",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("ATK"))));
            name+= "и атаки";
        }
        else
        {
            item_characteristics.insert(std::make_pair("ARM",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("ARM"))));
            name += "и защиты";
        }

        if(base == 1)
        {
            item_characteristics.insert(std::make_pair("STR",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("STR"))));
            name += "(STR)";
        }
        else if(base == 2)
        {
            item_characteristics.insert(std::make_pair("AGIL",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("AGIL"))));
            name+="(AGIL)";
        }
        else
        {
            item_characteristics.insert(std::make_pair("INT",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("INT"))));
            name += "(INT)";
        }
    }
    else if(turn_number > 50)
    {
        name = "Великое ожерелье ";
        if(spec == 1)
        {
            item_characteristics.insert(std::make_pair("PIERCE",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("STR"))));
            name += "пронзания ";
        }
        else if(spec == 2)
        {
            item_characteristics.insert(std::make_pair("CRIT_CH",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("CRIT_CH"))));
            name+= "шанса крит. удара ";
        }
        else
        {
            item_characteristics.insert(std::make_pair("CRIT_DMG",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("CRIT_DMG"))));
            name += "критческого удара ";
        }

        if(spec == 1)
        {
            item_characteristics.insert(std::make_pair("HP",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("HP"))));
            name += "и здоровья";
        }
        else if(spec == 2)
        {
            item_characteristics.insert(std::make_pair("CRIT_CH",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("ATK"))));
            name+= "и атаки";
        }
        else
        {
            item_characteristics.insert(std::make_pair("ARM",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("ARM"))));
            name += "и защиты";
        }

        if(base == 1)
        {
            item_characteristics.insert(std::make_pair("STR",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("STR"))));
            name += "(STR)";
        }
        else if(base == 2)
        {
            item_characteristics.insert(std::make_pair("AGIL",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("AGIL"))));
            name+="(AGIL)";
        }
        else
        {
            item_characteristics.insert(std::make_pair("INT",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("INT"))));
            name += "(INT)";
        }
    }
    else
    {
        name = "Ожерелье ";
        if(spec == 1)
        {
            item_characteristics.insert(std::make_pair("PIERCE",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("STR"))));
            name += "пронзания ";
        }
        else if(spec == 2)
        {
            item_characteristics.insert(std::make_pair("CRIT_CH",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("CRIT_CH"))));
            name+= "шанса крит. удара ";
        }
        else
        {
            item_characteristics.insert(std::make_pair("CRIT_DMG",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("CRIT_DMG"))));
            name += "критческого удара ";
        }

        if(spec == 1)
        {
            item_characteristics.insert(std::make_pair("HP",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("HP"))));
            name += "и здоровья";
        }
        else if(spec == 2)
        {
            item_characteristics.insert(std::make_pair("CRIT_CH",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("ATK"))));
            name+= "и атаки";
        }
        else
        {
            item_characteristics.insert(std::make_pair("ARM",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("ARM"))));
            name += "и защиты";
        }

        if(base == 1)
        {
            item_characteristics.insert(std::make_pair("STR",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("STR"))));
            name += "(STR)";
        }
        else if(base == 2)
        {
            item_characteristics.insert(std::make_pair("AGIL",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("AGIL"))));
            name+="(AGIL)";
        }
        else
        {
            item_characteristics.insert(std::make_pair("INT",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("INT"))));
            name += "(INT)";
        }
    }

}

void Jewel::make_belt(int turn_number)
{
    int base = rand() % 3 + 1, spec = rand() % 3 + 1, utility = rand() % 3 + 1;
    item_id = base*100 + spec*10 + utility + 660;
    equipment_class = "украшение";
    type = "пояс";
    is_equiped = 0;
    if(turn_number < 20)
    {
        name = "Простой пояс ";
        if(spec == 1)
        {
            item_characteristics.insert(std::make_pair("HP",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("HP"))));
            name += "здоровья";
        }
        else if(spec == 2)
        {
            item_characteristics.insert(std::make_pair("CRIT_CH",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("ATK"))));
            name+= "атаки";
        }
        else
        {
            item_characteristics.insert(std::make_pair("ARM",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("ARM"))));
            name += "защиты";
        }

        if(base == 1)
        {
            item_characteristics.insert(std::make_pair("STR",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("STR"))));
            name += "(STR)";
        }
        else if(base == 2)
        {
            item_characteristics.insert(std::make_pair("AGIL",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("AGIL"))));
            name+="(AGIL)";
        }
        else
        {
            item_characteristics.insert(std::make_pair("INT",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T1")->get_value("INT"))));
            name += "(INT)";
        }
    }
    else if(turn_number > 50)
    {
        name = "Великий пояс ";
        if(spec == 1)
        {
            item_characteristics.insert(std::make_pair("HP",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("HP"))));
            name += "здоровья";
        }
        else if(spec == 2)
        {
            item_characteristics.insert(std::make_pair("CRIT_CH",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("ATK"))));
            name+= "атаки";
        }
        else
        {
            item_characteristics.insert(std::make_pair("ARM",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("ARM"))));
            name += "защиты";
        }

        if(base == 1)
        {
            item_characteristics.insert(std::make_pair("STR",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("STR"))));
            name += "(STR)";
        }
        else if(base == 2)
        {
            item_characteristics.insert(std::make_pair("AGIL",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("AGIL"))));
            name+="(AGIL)";
        }
        else
        {
            item_characteristics.insert(std::make_pair("INT",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T3")->get_value("INT"))));
            name += "(INT)";
        }
    }
    else
    {
        name = "Пояс ";
        if(spec == 1)
        {
            item_characteristics.insert(std::make_pair("HP",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("HP"))));
            name += "здоровья";
        }
        else if(spec == 2)
        {
            item_characteristics.insert(std::make_pair("CRIT_CH",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("ATK"))));
            name+= "атаки";
        }
        else
        {
            item_characteristics.insert(std::make_pair("ARM",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("ARM"))));
            name += "защиты";
        }

        if(base == 1)
        {
            item_characteristics.insert(std::make_pair("STR",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("STR"))));
            name += "(STR)";
        }
        else if(base == 2)
        {
            item_characteristics.insert(std::make_pair("AGIL",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("AGIL"))));
            name+="(AGIL)";
        }
        else
        {
            item_characteristics.insert(std::make_pair("INT",std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object("T2")->get_value("INT"))));
            name += "(INT)";
        }
    }
}

Jewel::Jewel(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type) : Equipment(ID, name, equipment_class, characteristics, type) { }

Jewel::Jewel(std::string type, int turn_number) : Equipment()
{
    if(type == "кольцо")
        make_ring(turn_number);
    else if(type == "ожерелье")
        make_necklace(turn_number);
    else
        make_belt(turn_number);
}

//Weaponary
Weapon::Weapon(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type) : Equipment(ID, name, equipment_class, characteristics, type) { }

//Armourment
Armour::Armour(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type) : Equipment(ID, name, equipment_class, characteristics, type) { }

//Potion
void Potion::save(std::ofstream& out)
{
    Equipment::save(out);
    out.write((char*)& duration, sizeof(duration));

    size_t size_of_effect_name = effect_name.size() + 1;
    out.write((char*)& size_of_effect_name, sizeof(size_of_effect_name));
    out.write((char*)& effect_name, size_of_effect_name);
}

void Potion::load(std::ifstream& in)
{
    Equipment::load(in);
    in.read((char*)&duration, sizeof(duration));

    size_t size;
    in.read((char*)& size, sizeof(size));
    effect_name = std::string(size, ' ');
    in.read(effect_name.data(), size);
}

Potion::Potion(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type, int _duration, std::string _effect_name) : Equipment(ID, name, equipment_class, characteristics, type)
{
    duration = _duration;
    effect_name = _effect_name;
}

void Potion::dec_duration() { duration--; }

int Potion::get_duration() const { return duration; }

std::string Potion::get_effect_name() const { return effect_name; }
