
#include "DataBase.h"
Equipment::Equipment(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type)
{
    item_id = ID;
    this->name = name;
    item_characteristics = characteristics;
    this->type = type;
    this->equipment_class = equipment_class;
    is_equiped = 0;
}

Equipment::Equipment()
{
}

std::string Equipment::get_name() const { return name; }

const std::map<std::string, int>* Equipment::get_item_characteristics() const { return &item_characteristics; }

int Equipment::get_id() const { return item_id; }

bool Equipment::get_equiped() const { return is_equiped; }

std::string Equipment::get_type() const { return type; }

std::string Equipment::get_class() const
{
    return equipment_class;
}

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
Potion::Potion(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type, int _duration, std::string _effect_name) : Equipment(ID, name, equipment_class, characteristics, type)
{
    duration = _duration;
    effect_name = _effect_name;
}

void Potion::dec_duration() { duration--; }

int Potion::get_duration() const { return duration; }

std::string Potion::get_effect_name() const
{
    return effect_name;
}
