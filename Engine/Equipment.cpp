#include "DataBase.h"
#include "translator.h"

Equipment::Equipment(int ID, std::string _name, std::string _equipment_class, std::map<std::string, int> characteristics, std::string _type)
{
    item_id = ID;
    name = _name;
    item_characteristics = characteristics;
    type = _type;
    equipment_class = _equipment_class;
    is_equiped = false;
    is_front_equiped = false;
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

bool Equipment::get_front_equiped() const
{
    return is_front_equiped;
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


//Jewellery
void Jewel::make_ring(int turn_number)
{
    if (turn_number < 1)
    {
        throw std::exception("turn number is less than 1");
    }

    int base = rand() % 3 + 1, spec = rand() % 3 + 1, utility = rand() % 3 + 1;
    item_id = base*100 + spec*10 + utility + 600;
    equipment_class = "jewel";
    type = "ring";
    is_equiped = 0;

    std::string tier = "";

    if (turn_number < 20) // определяет тир кольца
    {
        tier = "T1";
        name = QObject::tr("Lesser ring").toStdString();
    }
    else if (turn_number > 50)
    {
        tier = "T3";
        name = QObject::tr("Great ring").toStdString();
    }
    else
    {
        tier = "T2";
        name = QObject::tr("Ring").toStdString();
    }

    std::string special = "";

    switch (spec) // определяет атакующее свойство кольца
    {
    case 1: name  += " " + QObject::tr("of piercing").toStdString();
        special = "PIERCE";
        break;
    case 2: name += " " + QObject::tr("of critical chance").toStdString();
        special = "CRIT_CH_FLAT";
        break;
    case 3: name += " " + QObject::tr("of critical damage").toStdString();
        special = "CRIT_DMG_FLAT";
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
    item_characteristics.insert(std::make_pair(special ,DataBase::get_DataBase()->get_jewellery_stats()[tier][special]));
    item_characteristics.insert(std::make_pair(basa,DataBase::get_DataBase()->get_jewellery_stats()[tier][basa]));
}

void Jewel::make_necklace(int turn_number)
{
    if (turn_number < 1)
    {
        throw std::exception("< 1???");
    }

    int base = rand() % 3 + 1, spec = rand() % 3 + 1, utility = rand() % 3 + 1;
    item_id = base*100 + spec*10 + utility + 630;
    equipment_class = "jewel";
    type = "necklace";
    is_equiped = 0;

    std::string tier = "";

    if (turn_number < 20) // определяет тир ожерелья
    {
        tier = "T1";
        name = QObject::tr("Lesser necklace").toStdString();
    }
    else if (turn_number > 50)
    {
        tier = "T3";
        name = QObject::tr("Great necklace").toStdString();
    }
    else
    {
        tier = "T2";
        name = QObject::tr("Necklace").toStdString();
    }

    std::string special = "";

    switch (spec) // определяет атакующее свойство ожерелья
    {
    case 1: name  += " " + QObject::tr("of piercing").toStdString() + " ";
        special = "PIERCE";
        break;
    case 2: name += " " + QObject::tr("of critical chance").toStdString() + " ";
        special = "CRIT_CH_FLAT";
        break;
    case 3: name += " " + QObject::tr("of critical damage").toStdString() + " ";
        special = "CRIT_DMG_FLAT";
        break;
    }

    std::string util = "";

    switch (utility) // определяет полезное свойство ожерелья
    {
    case 1: name += " " + QObject::tr("and health").toStdString();
        util = "HP";
        break;
    case 2: name += " " + QObject::tr("and attack").toStdString();
        util = "ATK_MULTI";
        break;
    case 3: name += " " + QObject::tr("and deffence").toStdString();
        util = "ARM_MULTI";
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
    item_characteristics.insert(std::make_pair(special ,DataBase::get_DataBase()->get_jewellery_stats()[tier][special]));
    item_characteristics.insert(std::make_pair(basa, DataBase::get_DataBase()->get_jewellery_stats()[tier][basa]));
    item_characteristics.insert(std::make_pair(util, DataBase::get_DataBase()->get_jewellery_stats()[tier][util]));
}

void Jewel::make_belt(int turn_number)
{
    if (turn_number < 1)
    {
        throw std::exception("номер хода не может быть меньше 1");
    }

    int base = rand() % 3 + 1, spec = rand() % 3 + 1, utility = rand() % 3 + 1;
    item_id = base*100 + spec*10 + utility + 660;
    equipment_class = "jewel";
    type = "belt";
    is_equiped = 0;

    std::string tier;

    if (turn_number < 20) // определяет тир пояса
    {
        tier = "T1";
        name = "Simple belt";
    }
    else if (turn_number > 50)
    {
        tier = "T3";
        name = "Great belt";
    }
    else
    {
        tier = "T2";
        name = "Belt";
    }

    std::string util = "";

    switch (utility) // определяет полезное свойство пояса
    {
    case 1: name += " " + QObject::tr("of health").toStdString();
        util = "HP";
        break;
    case 2: name += " " + QObject::tr("of attack").toStdString();
        util = "ATK_MULTI";
        break;
    case 3: name += " " + QObject::tr("of deffence").toStdString();
        util = "ARM_MULTI";
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
    item_characteristics.insert(std::make_pair(basa, DataBase::get_DataBase()->get_jewellery_stats()[tier][basa]));
    item_characteristics.insert(std::make_pair(util, DataBase::get_DataBase()->get_jewellery_stats()[tier][util]));
}

Jewel::Jewel(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type)
    : Equipment(ID, name, equipment_class, characteristics, type)
{

}

Jewel::Jewel(std::string type, int turn_number) : Equipment()
{
    if(type == "ring")
        make_ring(turn_number);
    else if(type == "necklace")
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
}

