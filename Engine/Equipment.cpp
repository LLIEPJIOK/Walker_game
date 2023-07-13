#include "DataBase.h"

Equipment::Equipment(int ID, std::string _name, std::string _equipment_class, std::map<std::string, int> characteristics, std::string _type)
{
    item_id = ID;
    name = _name;
    item_characteristics = characteristics;
    type = _type;
    equipment_class = _equipment_class;
    is_equiped = 0;
}

Equipment::Equipment()
{
}

std::string Equipment::get_name() const
{
    return name;
}

const std::map<std::string, int>* Equipment::get_item_characteristics() const
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

std::string Equipment::get_type() const
{
    return type;
}

std::string Equipment::get_class() const
{
    return equipment_class;
}

void Equipment::change_equiped()
{
    is_equiped = !is_equiped;
}


//Jewellery
void Jewel::make_ring(int turn_number)
{
    if (turn_number < 1)
    {
        throw std::exception("номер хода не может быть меньше 1");
    }

    int base = rand() % 3 + 1, spec = rand() % 3 + 1, utility = rand() % 3 + 1;
    item_id = base*100 + spec*10 + utility + 600;
    equipment_class = "украшение";
    type = "кольцо";
    is_equiped = 0;

    std::string tier = "";

    if (turn_number < 20) // определяет тир кольца
    {
        tier = "T1";
        name = "Малое кольцо ";
    }
    else if (turn_number > 50)
    {
        tier = "T3";
        name = "Великое кольцо ";
    }
    else
    {
        tier = "T2";
        name = "Кольцо ";
    }

    std::string special = "";

    switch (spec) // определяет атакующее свойство кольца
    {
    case 1: name  += "пронзания ";
        special = "PIERCE";
        break;
    case 2: name += "шанса крит. удара ";
        special = "CRIT_CH";
        break;
    case 3: name += "крит. удара ";
        special = "CRIT_DMG";
        break;
    }

    std::string basa ="";

    switch (base) // определяет атрибут в кольце
    {
    case 1: name += "(STR)";
        basa = "STR";
        break;
    case 2: name += "(AGIL)";
        basa = "AGIL";
        break;
    case 3: name += "(INT)";
        basa = "INT";
        break;
    }

    // заполнение характеристик
    item_characteristics.insert(std::make_pair(special ,std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object(tier)->get_value(special))));
    item_characteristics.insert(std::make_pair(basa,std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object(tier)->get_value(basa))));
}

void Jewel::make_necklace(int turn_number)
{
    if (turn_number < 1)
    {
        throw std::exception("номер хода не может быть меньше 1");
    }

    int base = rand() % 3 + 1, spec = rand() % 3 + 1, utility = rand() % 3 + 1;
    item_id = base*100 + spec*10 + utility + 630;
    equipment_class = "украшение";
    type = "ожерелье";
    is_equiped = 0;

    std::string tier = "";

    if (turn_number < 20) // определяет тир ожерелья
    {
        tier = "T1";
        name = "Малое ожерелье ";
    }
    else if (turn_number > 50)
    {
        tier = "T3";
        name = "Великое ожерелье ";
    }
    else
    {
        tier = "T2";
        name = "Ожерелье ";
    }

    std::string special = "";

    switch (spec) // определяет атакующее свойство ожерелья
    {
    case 1: name  += "пронзания ";
        special = "PIERCE";
        break;
    case 2: name += "шанса крит. удара ";
        special = "CRIT_CH";
        break;
    case 3: name += "крит. удара ";
        special = "CRIT_DMG";
        break;
    }

    std::string util = "";

    switch (utility) // определяет полезное свойство ожерелья
    {
    case 1: name += "и здоровья";
        util = "HP";
        break;
    case 2: name += "и атаки";
        util = "ATK";
        break;
    case 3: name += "и защиты";
        util = "ARM";
        break;
    }

    std::string basa ="";

    switch (base) // определяет атрибут ожерелья
    {
    case 1: name += "(STR)";
        basa = "STR";
        break;
    case 2: name += "(AGIL)";
        basa = "AGIL";
        break;
    case 3: name += "(INT)";
        basa = "INT";
        break;
    }

    // заполнение характеристик
    item_characteristics.insert(std::make_pair(special ,std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object(tier)->get_value(special))));
    item_characteristics.insert(std::make_pair(basa,std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object(tier)->get_value(basa))));
    item_characteristics.insert(std::make_pair(util,std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object(tier)->get_value(util))));
}

void Jewel::make_belt(int turn_number)
{
    if (turn_number < 1)
    {
        throw std::exception("номер хода не может быть меньше 1");
    }

    int base = rand() % 3 + 1, spec = rand() % 3 + 1, utility = rand() % 3 + 1;
    item_id = base*100 + spec*10 + utility + 660;
    equipment_class = "украшение";
    type = "пояс";
    is_equiped = 0;

    std::string tier;

    if (turn_number < 20) // определяет тир пояса
    {
        tier = "T1";
        name = "Простой пояс ";
    }
    else if (turn_number > 50)
    {
        tier = "T3";
        name = "Великий пояс ";
    }
    else
    {
        tier = "T2";
        name = "Пояс ";
    }

    std::string util = "";

    switch (utility) // определяет полезное свойство пояса
    {
    case 1: name += "здоровья";
        util = "HP";
        break;
    case 2: name += "атаки";
        util = "ATK";
        break;
    case 3: name += "защиты";
        util = "ARM";
        break;
    }

    std::string basa ="";

    switch (base) // определяет атрибут пояса
    {
    case 1: name += "(STR)";
        basa = "STR";
        break;
    case 2: name += "(AGIL)";
        basa = "AGIL";
        break;
    case 3: name += "(INT)";
        basa = "INT";
        break;
    }

    // заполнение характеристик
    item_characteristics.insert(std::make_pair(basa,std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object(tier)->get_value(basa))));
    item_characteristics.insert(std::make_pair(util,std::stoi(DataBase::get_DataBase()->get_jewellery_stats()->get_object(tier)->get_value(util))));
}

Jewel::Jewel(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type)
    : Equipment(ID, name, equipment_class, characteristics, type)
{

}

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
Weapon::Weapon(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type)
    : Equipment(ID, name, equipment_class, characteristics, type)
{

}

//Armourment
Armour::Armour(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type)
    : Equipment(ID, name, equipment_class, characteristics, type)
{

}

//Potion
Potion::Potion(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type, int _duration, std::string _effect_name)
    : Equipment(ID, name, equipment_class, characteristics, type)
{
    duration = _duration;
    effect_name = _effect_name;
}

void Potion::dec_duration()
{
    duration--;
}

int Potion::get_duration() const
{
    return duration;
}

std::string Potion::get_effect_name() const
{
    return effect_name;
}
