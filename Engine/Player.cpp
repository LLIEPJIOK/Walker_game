#include "Player.h"
#include "DataBase.h"
#include "Effect.h"
#include "Turn.h"

#define seq DataBase::get_DataBase()->get_sequence()
int Player::CURRENT_ID = 0;

void Player::save_all_items(std::ofstream &out)
{
    //размер контейнера weaponary
    size_t size = weaponary.size();
    //запись размера weaponary
    out.write((char*)& size, sizeof(size));
    //цикл для записи каждого объекта класса Weapon
    for(const auto &i : weaponary)
    {
        //вызов метода записи у объекта класса Weapon
        i->save(out);
    }

    //размер контейнера armourment
    size = armourment.size();
    //запись размера armourment
    out.write((char*)& size, sizeof(size));
    //цикл для записи каждого объекта класса Armour
    for(const auto &i : armourment)
    {
        //вызов метода записи у объекта класса Armour
        i->save(out);
    }

    //размер контейнера jewellery
    size = jewellery.size();
    //запись размера jewellery
    out.write((char*)& size, sizeof(size));
    //цикл для записи каждого объекта класса Jewel
    for(const auto &i : jewellery)
    {
        //вызов метода записи у объекта класса Jewel
        i->save(out);
    }

    //размер контейнера potions
    size = potions.size();
    //запись размера potions
    out.write((char*)& size, sizeof(size));
    //цикл для записи каждого объекта класса Potion
    for(const auto &i : potions)
    {
        //вызов метода записи у объекта класса Potion
        i->save(out);
    }

    //размер контейнера active_effects
    size = active_effects.size();
    //запись размера active_effects
    out.write((char*)& size, sizeof(size));
    //цикл для записи каждого объекта класса Effect и его наследников
    for(const auto &i : active_effects)
    {
        //вызов метода записи у объектов класса Effect и его наследников
        i->save(out);
    }
}

void Player::load_all_items(std::ifstream &in)
{
    //переменная для размеров контейнеров
    size_t size;
    //чтение размера контейнера weaponary
    in.read((char*)& size, sizeof(size));
    //очистка контейнера weaponary
    weaponary.clear();
    //цикл для заполенения контейнера данными
    for(int i = 0; i < size; i++)
    {
        //создание нового объекта класса Weapon без значений
        Weapon* item = new Weapon();
        //вызов метода чтения объекта класса Weapon
        item->load(in);
        //добавление в контейнер указателя на объект класса Weapon
        weaponary.insert(item);
    }

    //чтение размера контейнера armourment
    in.read((char*)& size, sizeof(size));
    //очистка контейнера armourment
    armourment.clear();
    //цикл для заполнения контейнера данными
    for(int i = 0; i < size; i++)
    {
        //создание нового объекта класса Armour без значений
        Armour* item = new Armour();
        //вызов метода чтения объекта класса Armour
        item->load(in);
        //добавление в контейнер указателя на объект класса Armour
        armourment.insert(item);
    }

    //чтение размера контейнера jewellery
    in.read((char*)& size, sizeof(size));
    //очистка контейнера jewellery
    jewellery.clear();
    //цикл для заполнения контейнера данными
    for(int i = 0; i < size; i++)
    {
        //создание нового объекта класса Jewel без значений
        Jewel* item = new Jewel();
        //вызов метода чтения объекта класса Jewel
        item->load(in);
        //добавление в контейнер указателя на объект класса Jewel
        jewellery.insert(item);
    }

    //чтение размера контейнера potions
    in.read((char*)& size, sizeof(size));
    //очистка контейнера potions
    potions.clear();
    //цикл для заполнения контейнера данными
    for(int i = 0; i < size; i++)
    {
        //создание нового объекта класса Potion
        Potion* item = new Potion();
        //вызов метода чтения объекта класса Potion
        item->load(in);
        //добавление в контейнер указателя на объект класса Potion
        potions.insert(item);
    }

    //чтение размера контейнера active_effects
    in.read((char*)& size, sizeof(size));
    //очистка контейнера active_effects
    active_effects.clear();
    //цикл для заполения контейнера данными
    for(int i = 0; i < size; i++)
    {
        //чтение поля name объекта класса Effect для инициализации указателя
        std::string s = Effect::read_name(in);
        //инициализация указателя новым объектом наследника класса Effect
        Effect* item = All_effects::get_effects_data()->get_effects()->at(s);
        //вызов метода чтения объекта класса Effect или его наследника
        item->load(in);
        //добавление в контейнер указателя на объект наследника класса Effect
        active_effects.push_back(item);
    }
}

void Player::load_equiped_items()
{
    //цикл для заполнения контейнера equiped_weaponary
    for(auto & i : weaponary)
    {
        //проверка на экипированность предмета
        if(i->get_equiped())
        {
            //присваивание указателя по ключу
            equiped_weaponary[i->get_type()] = i;
        }
    }

    //цикл для заполнения контейнера equiped_armourment
    for(auto& i : armourment)
    {
        //проверка на экипированность предмета
        if (i->get_equiped())
        {
            //присваивание указателя по ключу
            equiped_armourment[i->get_type()] = i;
        }
    }

    //цикл для заполнения контейнера equiped_jewellery
    for(auto& i : jewellery)
    {
        //проверка на экипированность предмета
        if(i->get_equiped())
        {
            //присваивание указателя по ключу
            equiped_jewellery[i->get_type()] = i;
        }
    }
}

Player::Player(const std::string& _name) : PLAYER_ID(++CURRENT_ID)
{
	name = _name;
    x = y = 0;
    previous_direction = std::make_pair(0, 0);
    // Итоговые хар-ки, используемые в рассчетах при атаке, ивентах и получении урона:

    // Здоровье, оно же ОЗ и макс. ОЗ
    characteristics["HP"] = 100;
    characteristics["MAX_HP"] = 100;

    // Атрибуты, влияют на прохождение ивентов, а также на ваши ОЗ, криты
    characteristics["AGIL"] = 1;
    characteristics["STR"] = 1;
    characteristics["INT"] = 1;

    // хар-ки атаки: атк, крит шанс, крит урон, расстояние атаки и прорубающий урон (сквозь ARM)
    characteristics["ATK"] = 10;
    characteristics["CRIT_CH"] = 5;
    characteristics["CRIT_DMG"] = 100;
    characteristics["RNG"] = 1;
    characteristics["PIERCE"] = 0;


    // Хар-ки, влияющие на получаемый урон: броня, физ. сопротивление
    characteristics["ARM"] =  0;
    characteristics["PIERCE_ARM"] = 0;

    // хар-ки кубиков
    characteristics["DQNT"] = 1; // кол-во кубиков (влияет только на передвижение)
    characteristics["ROLL_MOD"] = 0; // модификатор ролла при передвижении
    characteristics["EVENT_ROLL_MOD"] = 0; // модификатор ролла в ивенте

    // я не знаю что это...
	characteristics["RGN"] = 0;

    // здоровье армора, это баг, надо фиксить
    characteristics["ARM_VIT"] = 1;

    // хар-ки, участвующие в рассчете итоговых:

    // ATK:
    characteristics["ATK_FLAT"] = 10; // плоское значение атаки
    characteristics["ATK_MULTI"] = 100; // процентный модификатор атаки

    // CRIT_CH:
    characteristics["CRIT_CH_FLAT"] = 5; // обычный крит. шанс
    characteristics["CRIT_CH_AGIL"] = characteristics["AGIL"] / 5; // шанс крита, полученный за счет ловкости

    // CRIT_DMG:
    characteristics["CRIT_DMG_FLAT"] = 100; // обычный мультипликатор АТК при крит. ударе
    characteristics["CRIT_DMG_INT"] = characteristics["INT"] / 5; // крит урон, полученный за счет интеллекта

    // ARM:
    characteristics["ARM_FLAT"] = 0; // плоская броня
    characteristics["ARM_MULTI"] = 100; // мультипликатор брони

    // HP
    //characteristics["HP_FLAT"] = 5; // плоское ОЗ
    //characteristics["HP_MULTI"] = 100; // мультипликатор ОЗ
    characteristics["HP_MAX_STR"] = characteristics["STR"] * 2; // ОЗ, полученные за счет силы
    characteristics["HP_MAX_FLAT"] = 100;

	//equiped_Armourment
    equiped_armourment["нагрудник"] = nullptr;
    equiped_armourment["шлем"] = nullptr;
    equiped_armourment["перчатки"] = nullptr;
    equiped_armourment["поножи"] = nullptr;
    equiped_armourment["сапоги"] = nullptr;

	//equiped_Weaponary
    equiped_weaponary["основная_рука"] = nullptr;
    equiped_weaponary["неосновная_рука"] = nullptr;

	//equiped_Jewellery
    equiped_jewellery["первое_кольцо"] = nullptr;
    equiped_jewellery["второе_кольцо"] = nullptr;
    equiped_jewellery["ожерелье"] = nullptr;
    equiped_jewellery["пояс"] = nullptr;

    killed_player = -1;
}

Player::~Player()
{
    for (auto& i : weaponary)
		delete i;
    for (auto& i : armourment)
		delete i;
    for (auto& i : potions)
		delete i;
    for (auto& i : jewellery)
		delete i;

    CURRENT_ID--;
}

int Player::get_id() const
{
    return PLAYER_ID;
}

std::string Player::get_name() const
{
    return name;
}

int Player::get_killed_player()
{
    return killed_player;
}

void Player::set_killed_player(int id)
{
    killed_player = id;
}

int Player::get_x() const
{
    return x;
}

void Player::set_x(const int& _x)
{
    x = _x;
}

int Player::get_y() const
{
    return y;
}

void Player::set_y(const int& _y)
{
    y = _y;
}

std::pair<int, int> Player::get_previous_direction() const
{
    return previous_direction;
}

void Player::set_previous_direction(const std::pair<int, int>& dir)
{
    previous_direction = dir;
}

void Player::change_x(const bool& is_right)
{
	if (is_right)
	{
		++x;
		return;
	}
	--x;
}

void Player::change_y(const bool& is_up)
{
	if (is_up)
	{
		--y;
		return;
    }
    ++y;
}

void Player::update_chars()
{
    // обновляем макс. хп
    int hp_str = characteristics["STR"] * 2;
    characteristics["HP_MAX_STR"] = hp_str;
    characteristics["MAX_HP"] = characteristics["HP_MAX_FLAT"] + hp_str;
    if (characteristics["HP"] > characteristics["MAX_HP"])
        characteristics["HP"] = characteristics["MAX_HP"];

    // обновляем АТК
    characteristics["ATK"] = (characteristics["ATK_FLAT"] * characteristics["ATK_MULTI"]) / 100;

    // обновляем криты
    int cr_ch_agil = characteristics["AGIL"];
    int cr_dmg_int = characteristics["INT"] * 2;

    characteristics["CRIT_CH_AGIL"] = cr_ch_agil;
    characteristics["CRIT_DMG_INT"] = cr_dmg_int;

    characteristics["CRIT_CH"] = cr_ch_agil + characteristics["CRIT_CH_FLAT"];
    characteristics["CRIT_DMG"] = cr_dmg_int + characteristics["CRIT_DMG_FLAT"];

    // обновляем защиту
    int arm = (characteristics["ARM_FLAT"] * characteristics["ARM_MULTI"]) / 100;
    int pierce_arm= sqrt(sqrt(arm * arm * arm)); // perfect formula, no way
    if (pierce_arm > 75)
        pierce_arm = 75;
    characteristics["PIERCE_ARM"] = pierce_arm;
    characteristics["ARM"] = arm;
}

void Player::set_current_id(int id)
{
    CURRENT_ID = id;
}

std::map<std::string, int>& Player::get_characteristics()
{
    return characteristics;
}

std::multiset <Weapon*, Equipment_Comparator>* Player::get_weaponary()
{
    return &weaponary;
}

std::multiset <Armour*, Equipment_Comparator>* Player::get_armourment()
{
    return &armourment;
}

std::multiset <Potion*, Equipment_Comparator>* Player::get_potions()
{
    return &potions;
}

std::multiset <Jewel*, Equipment_Comparator>* Player::get_jewellery()
{
    return &jewellery;
}

std::vector <Effect*>* Player::get_active_effects()
{
    return &active_effects;
}

std::map<std::string, Weapon*>* Player::get_equiped_weaponary()
{
    return &equiped_weaponary;
}

std::map<std::string, Armour*>* Player::get_equiped_armourment()
{
    return &equiped_armourment;
}

std::map<std::string, Jewel*>* Player::get_equiped_jewellery()
{
    return &equiped_jewellery;
}

void Player::attack(Player* pl)
{
    int basa = characteristics["ATK"];
    int crit = rand() % 100 + 1;
    int multi = 100;
    if(characteristics["CRIT_CH"] >= crit)
        multi += characteristics["CRIT_DMG"];
    basa *= multi;
    int pdmg = characteristics["PIERCE"];
    basa /= 100;

    if(basa <= pl->get_characteristics().at("ARM")) // anti-healing
        basa *= 0;
    else
        basa -= pl->get_characteristics().at("ARM");

    if(pdmg > characteristics["ATK"] - basa) // check of overnatural pierce
        pdmg = characteristics["ATK"] - basa;

    pl->get_characteristics().at("HP") -= basa + pdmg;
    killed_player = pl->die();
}

void Player::use_potion(Potion* potion)
{
    if (potion->get_type() == "мгновенное")
    {
        for (const auto& i : *potion->get_item_characteristics()) // увеличение хар-к навсегда
        {
            std::string ch = i.first;
            int value = i.second;
            if (ch == "HP" && characteristics["HP"] + value >= characteristics["MAX_HP"]) // проверка на избыток здоровья
            {
                characteristics["HP"] = characteristics["MAX_HP"];
                continue;
            }

            characteristics[ch] += value;
        }

        if(potion->get_effect_name() != "нет") // если мгновенное зелье по-совместительству имеет эффект
            All_effects::get_effects_data()->get_effects()->at(potion->get_effect_name())->apply_effect(*this, potion->get_duration());
    }
    else // наложение соотв. эффекта на игрока
    {
        All_effects::get_effects_data()->get_effects()->at(potion->get_effect_name())->apply_effect(*this, potion->get_duration());
	}

    update_chars();
    potions.erase(potion);
	delete potion;
}

int Player::die()
{
	if (characteristics["HP"] <= 0)
	{
        int to_return = 0;
        auto s = DataBase::get_DataBase()->get_sequence();
        for(auto i = s->begin(); i !=s->end(); i++)
        {
            if((*i)->PLAYER_ID == this->PLAYER_ID)
            {

                s->erase(i);
                break;
            }
            to_return++;
        }
        delete this;
        return to_return;
	}
    return -1;
}

void Player::save(std::ofstream& out)
{
    //размер name
    size_t size = name.size() + 1;
    //запись размера name
    out.write((char*)& size, sizeof(size));
    //запись name
    out.write(name.c_str(), size);

    //запись первого значения previous_direction
    out.write((char*)& previous_direction.first, sizeof(previous_direction.first));
    //запись второго значения previous_direction
    out.write((char*)& previous_direction.second, sizeof(previous_direction.second));

    //запись x
    out.write((char*)& x, sizeof(x));
    //запись y
    out.write((char*)& y, sizeof(y));

    //размер контейнера characteristics
    size = characteristics.size();
    //запись размера characteristics
    out.write((char*)& size, sizeof(size));
    //цикл для записи контейнера
    for(const auto &i : characteristics)
    {
        //размер ключа
        size_t string_size = i.first.size() + 1;
        //запись размера ключа
        out.write((char*)& string_size, sizeof(string_size));
        //запись ключа
        out.write(i.first.c_str(), string_size);

        //запись значения
        out.write((char*)& i.second, sizeof(i.second));
    }

    //вызов метода записи всех предметов
    save_all_items(out);
}

void Player::load(std::ifstream &in)
{
    //переменная для размера строки и контейнера
    size_t size;
    //чтение размера строки
    in.read((char*) & size, sizeof(size));
    //присваивание name строки из пробелов длиной size-1, без /0
    name = std::string(size - 1, ' ');
    //чтение name
    in.read(name.data(), size);

    //чтение первого значния previous_direction
    in.read((char*)& previous_direction.first, sizeof(previous_direction.first));
    //чтение второго значения previous_direction
    in.read((char*)& previous_direction.second, sizeof(previous_direction.second));

    //чтение x
    in.read((char*)& x, sizeof(x));
    //чтение y
    in.read((char*)& y, sizeof(y));

    //чтение размера контейнера characteristics
    in.read((char*)& size, sizeof(size));
    //очистка контейнера characteristics
    characteristics.clear();
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
        //чтение значения
        in.read((char*)& value, sizeof(value));

        //добавление или изменение пары ключ-значение
        characteristics[key] = value;
    }

    //вызов метода чтения всех предметов
    load_all_items(in);
    //вызов метода заполнения всех экипированныех предметов
    load_equiped_items();
}

void Player::process_active_effects() // производит исполнение эффектов, уменьшение их времени действия и увеличения кол-ва стаков
{
    for (auto i = active_effects.begin(); i!=active_effects.end();)
	{
		if (!(*i)->get_effect_duration())
		{
			(*i)->reverse_effect(*this);
            delete* i;
            i = active_effects.erase(i);
        }
        else
        {
			(*i)->execute_effect(*this);
            (*i)->dec_duration();
            (*i)->inc_counter();
            i++;
        }
    }

    update_chars();
}

void Player::equip_armour(Armour* armour, std::string place)
{
    if (equiped_armourment[place] != nullptr)
        unequip_item(equiped_armourment[place], place);
    equiped_armourment[place] = armour;
	armour->change_equiped();
	for (const auto& i : *armour->get_item_characteristics())
		characteristics[i.first] += i.second;

    update_chars();
}

void Player::equip_weapon(Weapon* weapon, std::string place)
{
    /*if (weapon->get_type() == "one_handed")
	{
		if (equiped_weaponary["two_handed"] != nullptr)
		{
			unequip_item(equiped_weaponary["two_handed"]);
			equiped_weaponary["main_hand"] = weapon;
		}
		else if (equiped_weaponary["main_hand"] != nullptr)
		{
			if (equiped_weaponary["non_dominant_hand"] != nullptr)
			{
                std::string choice;
				if (choice == "exit")
					return;
				unequip_item(equiped_weaponary[choice]);
				equiped_weaponary[choice] = weapon;
			}
			else
				equiped_weaponary["non_dominant_hand"] = weapon;
		}
		else
			equiped_weaponary["main_hand"] = weapon;
	}
	else
	{
		if (equiped_weaponary["two_handed"] != nullptr)
		{
			unequip_item(equiped_weaponary["two_handed"]);
		}
		else
		{
			if (equiped_weaponary["main_hand"] != nullptr && equiped_weaponary["non_dominant_hand"] != nullptr)
			{
				unequip_item(equiped_weaponary["main_hand"]);
				unequip_item(equiped_weaponary["non_dominant_hand"]);
			}
			else if (equiped_weaponary["main_hand"] != nullptr)
				unequip_item(equiped_weaponary["main_hand"]);
			else
				unequip_item(equiped_weaponary["non_dominant_hand"]);
		}
		equiped_weaponary["two_handed"] = weapon;
    }*/
    if (equiped_weaponary[place] != nullptr)
        unequip_item(equiped_weaponary[place], place);
    equiped_weaponary[place] = weapon;
	weapon->change_equiped();
	for (const auto& i : *weapon->get_item_characteristics())
		characteristics[i.first] += i.second;

    update_chars();
}

void Player::equip_jewel(Jewel* jewel, std::string place)
{

    /*if (jewel->get_type() == "ring")
	{
		if (equiped_jewellery["first_ring"] != nullptr)
		{
			if (equiped_jewellery["second_ring"] != nullptr)
			{
                std::string choice;
				if (choice == "exit")
					return;
				unequip_item(equiped_jewellery[choice]);
				equiped_jewellery[choice] = jewel;
			}
			else
				equiped_jewellery["second_ring"] = jewel;
		}
		else
			equiped_jewellery["first_ring"] = jewel;
	}
	else if (equiped_jewellery[jewel->get_type()] != nullptr)
	{
		unequip_item(equiped_jewellery[jewel->get_type()]);
		equiped_jewellery[jewel->get_type()] = jewel;
    }*/
    if (equiped_jewellery[place] != nullptr)
        unequip_item(equiped_jewellery[place], place);
    equiped_jewellery[place] = jewel;
	jewel->change_equiped();
	for (const auto& i : *jewel->get_item_characteristics())
		characteristics[i.first] += i.second;

    update_chars();
}

void Player::unequip_item(Equipment* equipment, std::string place)
{
	std::string type = equipment->get_type();
	std::string equipment_class = equipment->get_class();
    if (equipment_class == "оружие")
	{
        /*if (equipment->get_type() == "one_handed")
		{
			if (equiped_weaponary["main_hand"] == equipment)
				equiped_weaponary["main_hand"] = nullptr;
			else
				equiped_weaponary["non_dominant_hand"] = nullptr;
		}
		else
            equiped_weaponary[equipment->get_type()] = nullptr;*/
        equiped_weaponary[place] = nullptr;
	}
    else if (equipment_class == "броня")
    {
        equiped_armourment[place] = nullptr;
	}
    else if (equipment_class == "украшение")
	{
        /*if(type == "ring")
		{
			if (equiped_jewellery["first_ring"] == equipment)
				equiped_jewellery["first_ring"] = nullptr;
			else
				equiped_jewellery["second_ring"] = nullptr;
		}
		else
            equiped_jewellery[type] = nullptr;*/
        equiped_jewellery[place] = nullptr;
    }

    equipment->change_equiped();
    for (auto& i : *equipment->get_item_characteristics())
        characteristics[i.first] -= i.second;

    update_chars();
}

Equipment* Player::add_item(const std::string& equipment_id)
{
    if(equipment_id == "Загадочное кольцо")
    {
        Jewel* ring = new Jewel("кольцо", Turn::get_Turn()->get_turn_number());
        jewellery.insert(ring);
        return ring;
    }
    else if(equipment_id == "Загадочное ожерелье")
    {
        Jewel* necklace = new Jewel("ожерелье", Turn::get_Turn()->get_turn_number());
        jewellery.insert(necklace);
        return necklace;
    }
    else if(equipment_id == "Загадочный пояс")
    {
        Jewel* belt = new Jewel("пояс", Turn::get_Turn()->get_turn_number());
        jewellery.insert(belt);
        return belt;
    }


    std::map<std::string, int> item_characteristics;
    for (const auto& i : *DataBase::get_DataBase()->get_all_equipment_data()->get_object(equipment_id)->get_object("chars")->get_name_to_value())
        item_characteristics.emplace(std::make_pair(i.first, stoi(i.second)));
    std::string name = DataBase::get_DataBase()->get_all_equipment_data()->get_object(equipment_id)->get_value("name");
    int item_id = stoi(DataBase::get_DataBase()->get_all_equipment_data()->get_object(equipment_id)->get_value("ID"));
    std::string type = DataBase::get_DataBase()->get_all_equipment_data()->get_object(equipment_id)->get_value("type");
    std::string equipment_class = DataBase::get_DataBase()->get_all_equipment_data()->get_object(equipment_id)->get_value("class");

    if (equipment_class == "оружие")
    {
        Weapon* item = new Weapon(item_id, name, equipment_class, item_characteristics, type);
        weaponary.insert(item);
        return item;
    }
    if (equipment_class == "броня")
    {
        Armour* item = new Armour(item_id, name, equipment_class, item_characteristics, type);
        armourment.insert(item);
        return item;
    }

    int dur = stoi(DataBase::get_DataBase()->get_all_equipment_data()->get_object(equipment_id)->get_value("duration"));
    std::string effect_name = DataBase::get_DataBase()->get_all_equipment_data()->get_object(equipment_id)->get_value("effect_name");;
    Potion* item = new Potion(item_id, name, equipment_class, item_characteristics, type, dur, effect_name);
    potions.insert(item);
    return item;
}
