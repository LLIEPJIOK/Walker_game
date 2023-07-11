#include "Player.h"
#include "DataBase.h"
#include "Effect.h"
#include "Turn.h"

#define seq DataBase::get_DataBase()->get_sequence()
int Player::CURRENT_ID = 0;

Player::Player(const std::string& _name) : PLAYER_ID(++CURRENT_ID)
{
	name = _name;
    x = y = 0;
    previos_direction = std::make_pair(0, 0);
	// Characteristics
    characteristics["HP"] = 100;
	characteristics["MAX_HP"] = 100;

    characteristics["AGIL"] = 1;
    characteristics["STR"] = 1;
    characteristics["INT"] = 1;

    characteristics["ATK"] = 10;
    characteristics["ARM"] =  0;

    // рейндж
	characteristics["RNG"] = 1;

    // регенерация
	characteristics["RGN"] = 0;
    characteristics["ARM_VIT"] = 1;

    // количество кубиков
    characteristics["DQNT"] = 1;

    // блокирования прорубающего урона
    characteristics["PIERCE_ARM"] = 0;

    // прорубающий урон
    characteristics["PIERCE"] = 0;

    // криты
    characteristics["CRIT_CH"] = 5;
    characteristics["CRIT_DMG"] = 100;

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
	for (const auto& i : weaponary)
		delete i;
	for (const auto& i : armourment)
		delete i;
	for (const auto& i : potions)
		delete i;
	for (const auto& i : jewellery)
		delete i;
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
    return previos_direction;
}

void Player::set_previous_direction(const std::pair<int, int>& dir)
{
    previos_direction = dir;
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
		for (const auto& i : *potion->get_item_characteristics())
            characteristics[i.first] += i.second;
        if(potion->get_effect_name() != "нет")
            All_effects::get_effects_data()->get_effects()->at(potion->get_effect_name())->apply_effect(*this, potion->get_duration());
    }
	else
    {
        All_effects::get_effects_data()->get_effects()->at(potion->get_effect_name())->apply_effect(*this, potion->get_duration());
	}
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

void Player::process_active_effects()
{
	for (auto i = active_effects.begin(); i!=active_effects.end(); i++)
	{
		if (!(*i)->get_effect_duration())
		{
			(*i)->reverse_effect(*this);
            delete* i;
            active_effects.erase(i);
        }
		else
			(*i)->execute_effect(*this);
	}
		
}

void Player::equip_armour(Armour* armour, std::string place)
{
    if (equiped_armourment[place] != nullptr)
        unequip_item(equiped_armourment[place], place);
    equiped_armourment[place] = armour;
	armour->change_equiped();
	for (const auto& i : *armour->get_item_characteristics())
		characteristics[i.first] += i.second;
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
}

Equipment* Player::add_item(const std::string& equipment_id)
{
    if(equipment_id == "Загадочное кольцо")
    {
        Jewel* ring = new Jewel("кольцо", Turn::get_Turn()->get_turn_number());
        return ring;
    }
    else if(equipment_id == "Загадочное ожерелье")
    {
        Jewel* necklace = new Jewel("ожерелье", Turn::get_Turn()->get_turn_number());
        return necklace;
    }
    else if(equipment_id == "Загадочный пояс")
    {
        Jewel* belt = new Jewel("пояс", Turn::get_Turn()->get_turn_number());
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
