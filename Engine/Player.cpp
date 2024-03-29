#include "Player.h"
#include "DataBase.h"
#include "Effect.h"
#include "Engine/Transceiver.h"
#include "Turn.h"

#define seq DataBase::get_DataBase()->get_sequence()
int Player::CURRENT_ID = 0;

Player::Player(const std::string& _name) : PLAYER_ID(++CURRENT_ID)
{
    struct lesha{};

	name = _name;
    players_position = Coordinates::Hex<int> {0, 0};

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

    // здоровье армора, это баг, надо фиксить, а может и все равно...
    characteristics["ARM_VIT"] = 1;

    // хар-ки, участвующие в рассчете итоговых:

    // ATK:
    characteristics["ATK_FLAT"] = 10; // плоское значение атаки
    characteristics["ATK_MULTI"] = 100; // процентный модификатор атаки

    // CRIT_CH:
    characteristics["CRIT_CH_FLAT"] = 5; // обычный крит. шанс
    characteristics["CRIT_CH_AGIL"] = characteristics["AGIL"] / 5; // шанс крита, полученный за счет ловкости
    characteristics["CRIT_CH_MULTI"] = 100; // множитель крита

    // CRIT_DMG:
    characteristics["CRIT_DMG_FLAT"] = 100; // обычный мультипликатор АТК при крит. ударе
    characteristics["CRIT_DMG_INT"] = characteristics["INT"] / 5; // крит урон, полученный за счет интеллекта
    characteristics["CRIT_DMG_MULTI"] = 100; // множитель крита

    // ARM:
    characteristics["ARM_FLAT"] = 0; // плоская броня
    characteristics["ARM_MULTI"] = 100; // мультипликатор брони

    // HP
    //characteristics["HP_FLAT"] = 5; // плоское ОЗ
    //characteristics["HP_MULTI"] = 100; // мультипликатор ОЗ
    characteristics["HP_MAX_STR"] = characteristics["STR"] * 2; // макс. ОЗ, полученные за счет силы
    characteristics["HP_MAX_FLAT"] = 100;

    characteristics["HP"] = characteristics["MAX_HP"];

    // Экипированые предметы

	//equiped_Armourment
    equipped_items["cuirass"] = nullptr;
    equipped_items["helmet"] = nullptr;
    equipped_items["gloves"] = nullptr;
    equipped_items["leggings"] = nullptr;
    equipped_items["boots"] = nullptr;

	//equiped_Weaponary
    equipped_items["main_hand"] = nullptr;
    equipped_items["offhand"] = nullptr;

	//equiped_Jewellery
    equipped_items["first_ring"] = nullptr;
    equipped_items["second_ring"] = nullptr;
    equipped_items["necklace"] = nullptr;
    equipped_items["belt"] = nullptr;

    killed_player = -1;
}

void Player::update_chars()
{
    // обновляем макс. ОЗ, учитывая сопутствующие изменения ОЗ
    int cur_max_hp = characteristics["MAX_HP"];
    int hp_str = characteristics["STR"] * 2;
    characteristics["HP_MAX_STR"] = hp_str;
    int new_max_hp = characteristics["HP_MAX_FLAT"] + hp_str;
    int diff = new_max_hp - cur_max_hp;

    // если макс. ОЗ увеличилось, то мы увеличиваем ОЗ на разницу
    if (diff > 0)
        characteristics["HP"] += diff;

    characteristics["MAX_HP"] = new_max_hp;

    // если макс. здоровье уменьшилось, при этом ОЗ стало больше макс., мы опускаем ОЗ до макс
    if (characteristics["HP"] > characteristics["MAX_HP"])
        characteristics["HP"] = characteristics["MAX_HP"];

    // обновляем АТК
    characteristics["ATK"] = (characteristics["ATK_FLAT"] * characteristics["ATK_MULTI"]) / 100;

    // обновляем криты (верхнее зн. шанса = 85, крит урона - нет; нижнее зн. шанса = 0, урона = 100)
    int cr_ch_agil = characteristics["AGIL"];
    int cr_dmg_int = characteristics["INT"] * 2;

    characteristics["CRIT_CH_AGIL"] = cr_ch_agil;
    characteristics["CRIT_DMG_INT"] = cr_dmg_int;

    int total_ch = ((cr_ch_agil + characteristics["CRIT_CH_FLAT"]) * characteristics["CRIT_CH_MULTI"]) / 100;
    if (total_ch > 85)
        total_ch = 85;
    if (total_ch < 0)
        total_ch = 0;

    int total_dmg = ((cr_dmg_int + characteristics["CRIT_DMG_FLAT"]) * characteristics["CRIT_DMG_MULTI"]) / 100;
    if (total_dmg < 100)
        total_dmg = 100;

    characteristics["CRIT_CH"] = total_ch;
    characteristics["CRIT_DMG"] = total_dmg;

    // обновляем защиту (верхнее зн. физ. сопротивления = 75, брони - нет)
    int arm = (characteristics["ARM_FLAT"] * characteristics["ARM_MULTI"]) / 100;
    int pierce_arm;
    if (arm < 0)
        pierce_arm= -1 * sqrt(sqrt(-1 * (arm * arm * arm))); // perfect formula, no way maaaan
    else
        pierce_arm = sqrt(sqrt(arm * arm * arm));

    if (pierce_arm > 75)
        pierce_arm = 75;
    characteristics["PIERCE_ARM"] = pierce_arm;
    characteristics["ARM"] = arm;
}

Player::~Player()
{
    for (auto& i : items)
        delete i;

    items.clear();

    for (auto& i : potions)
        delete i;

    for (auto& i : active_effects)
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

Coordinates::Hex<int> Player::get_players_position() const
{
    return players_position;
}

void Player::set_players_position(const Coordinates::Hex<int> new_position)
{
    players_position = new_position;
}

int Player::get_killed_player()
{
    return killed_player;
}

void Player::set_killed_player(int id)
{
    killed_player = id;
}


void Player::set_current_id(int id)
{
    CURRENT_ID = id;
}

std::map<std::string, int>& Player::get_characteristics()
{
    return characteristics;
}

std::multiset <Equipment*, Equipment_Comparator>* Player::get_items()
{
    return &items;
}

std::multiset<Potion *, Equipment_Comparator> *Player::get_potions()
{
    return &potions;
}

std::vector <Effect*>* Player::get_active_effects()
{
    return &active_effects;
}

std::map<std::string, Equipment*>* Player::get_equipped_items()
{
    return &equipped_items;
}

std::pair<int, bool> Player::attack(Player* pl)
{
    double basa = characteristics["ATK"];
    int crit = rand() % 100 + 1;
    double multi = 100;
    if(characteristics["CRIT_CH"] >= crit)
        multi += characteristics["CRIT_DMG"];
    basa *= multi;
    double pdmg = characteristics["PIERCE"];
    basa /= 100;

    if(basa <= pl->get_characteristics().at("ARM")) // anti-healing
        basa *= 0;
    else
        basa -= pl->get_characteristics().at("ARM");

    if(pdmg > characteristics["ATK"] - basa) // check of overnatural pierce
        pdmg = characteristics["ATK"] - basa;

    pl->get_characteristics().at("HP") -= basa + pdmg;

    game_msg msg = {Transceiver::get_transceiver()->get_id(), pl->get_id() - 1, 14, static_cast<int>(basa + pdmg)};
    if (crit <= characteristics["CRIT_CH"])
        msg.buffer[0] = 1;

    Transceiver::get_transceiver()->send_msg(msg);

    killed_player = pl->die();

    return {static_cast<int>(basa + pdmg), crit <= characteristics["CRIT_CH"]};
}

void Player::use_potion(Potion* potion)
{
    game_msg msg = {Transceiver::get_transceiver()->get_id(), Transceiver::get_transceiver()->get_id(), 13, 0};
    std::string txt = potion->get_name();
    for (int i = 0; i < 127 && i < txt.size(); i++){
        msg.buffer[i] = txt[i];
    }

    Transceiver::get_transceiver()->send_msg(msg);

    if (potion->get_effect_name() == "dispell"){
        Effect eff(potion->get_effect_name(), this, potion->get_duration());
        eff.execute_effect();
    }
    else {
        if (potion->get_type() == "instant")
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

            if(potion->get_effect_name() != "none") // если зелье имеет эффект
            {
                Effect* eff = new Effect(potion->get_effect_name(), this, potion->get_duration());
                eff->apply_effect();
            }
        }
        else // наложение соотв. эффекта на игрока
        {
            Effect* eff = new Effect(potion->get_effect_name(), this, potion->get_duration());
            eff->apply_effect();
        }

        update_chars();
        potions.erase(potion);
        delete potion;
    }
}

void Player::use_potion(std::string potion_name)
{
    for (auto& i : potions){
        if (i->get_name() == potion_name){
            for (const auto& j : *i->get_item_characteristics()) // увеличение хар-к навсегда
            {
                std::string ch = j.first;
                int value = j.second;
                if (ch == "HP" && characteristics["HP"] + value >= characteristics["MAX_HP"]) // проверка на избыток здоровья
                {
                    characteristics["HP"] = characteristics["MAX_HP"];
                    continue;
                }

                characteristics[ch] += value;
            }

            Potion* pot = i;
            potions.erase(i);
            delete pot;
            update_chars();
            return;
        }
    }
}

int Player::die()
{
	if (characteristics["HP"] <= 0)
	{
        int to_return = 0;
        auto s = DataBase::get_DataBase()->get_sequence();
        for(auto i = s->begin(); i != s->end(); i++)
        {
            if (*i == nullptr){
                to_return++;
                continue;
            }
            if(*i == this)
            {
                break;
            }
            to_return++;
        }

        s->at(to_return) = nullptr;
        delete this;
        return to_return;
	}

    return -1;
}

void Player::process_active_effects() // производит исполнение эффектов, уменьшение их времени действия и увеличения кол-ва стаков
{
    for (auto i = active_effects.begin(); i!=active_effects.end();)
    {
        (*i)->execute_effect();
        (*i)->dec_duration();
        (*i)->inc_counter();
		if (!(*i)->get_effect_duration())
		{
            (*i)->reverse_effect(*this);
            delete* i;
            i = active_effects.erase(i);
            continue;
        }

        i++;
    }

    update_chars();
}

void Player::equip_item(Equipment *item, std::string place)
{
    if (equipped_items.find(place) == equipped_items.end())
        throw new std::invalid_argument("Place is invalid");

    if (equipped_items[place] != nullptr)
        unequip_item(place);

    equipped_items.at(place) = item;
    item->change_equiped();

    game_msg msg = {Transceiver::get_transceiver()->get_id(), Transceiver::get_transceiver()->get_id(), 12, 0};
    std::string txt = place;
    txt += ' ';
    txt += item->get_name();
    for (int i = 0; i < 127 && i < txt.size(); i++){
        msg.buffer[i] = txt[i];
    }

    Transceiver::get_transceiver()->send_msg(msg);


    for (const auto& i : *item->get_item_characteristics())
        characteristics[i.first] += i.second;

    update_chars();
}

void Player::equip_item(std::string place, std::string eq_name)
{
    for (auto& item : items){
        if (!item->get_equiped() && item->get_name() == eq_name) {
            equipped_items[place] = item;
            item->change_equiped();
            for (const auto& i : *item->get_item_characteristics())
                characteristics[i.first] += i.second;

            update_chars();

            break;
        }
    }
}

void Player::unequip_item(std::string place, bool send)
{
    if (send){
        game_msg msg = {Transceiver::get_transceiver()->get_id(), Transceiver::get_transceiver()->get_id(), 11, 0};
        std::string txt = place;
        for (int i = 0; i < 127 && i < txt.size(); i++){
            msg.buffer[i] = txt[i];
        }

        Transceiver::get_transceiver()->send_msg(msg);
    }


    if (equipped_items[place] == nullptr)
        return;

    Equipment* equipment = equipped_items[place];
    std::string type = equipment->get_type();
    std::string equipment_class = equipment->get_class();
    equipped_items[place] = nullptr;

    equipment->change_equiped();
    for (auto& i : *equipment->get_item_characteristics())
        characteristics[i.first] -= i.second;

    update_chars();
}

Equipment* Player::add_item(const std::string& equipment_id)
{
    if(equipment_id == "Mystery ring")
    {
        Jewel* ring = new Jewel("ring", Turn::get_Turn()->get_turn_number());
        items.insert(ring);
        return ring;
    }
    else if(equipment_id == "Mystery necklace")
    {
        Jewel* necklace = new Jewel("necklace", Turn::get_Turn()->get_turn_number());
        items.insert(necklace);
        return necklace;
    }
    else if(equipment_id == "Mystery belt")
    {
        Jewel* belt = new Jewel("belt", Turn::get_Turn()->get_turn_number());
        items.insert(belt);
        return belt;
    }

    std::unordered_map<std::string, int> item_characteristics;
    if (DataBase::get_DataBase()->get_all_equipment_data()[equipment_id].contains("chars")) {
        json k = DataBase::get_DataBase()->get_all_equipment_data().at(equipment_id).at("chars");
        for (auto& [key, value] : k.items()) {
            item_characteristics.emplace(std::make_pair(key, value));
        }
    } // access problems, thus copying into k;

    std::string name = DataBase::get_DataBase()->get_all_equipment_data().at(equipment_id).at("name");
    int item_id = DataBase::get_DataBase()->get_all_equipment_data().at(equipment_id).at("ID");
    std::string type = DataBase::get_DataBase()->get_all_equipment_data().at(equipment_id).at("type");
    std::string equipment_class = DataBase::get_DataBase()->get_all_equipment_data().at(equipment_id).at("class");

    if (equipment_class != "potion"){
        Equipment* item = new Equipment(item_id, name, equipment_class, item_characteristics, type);
        items.insert(item);
        return item;
    }

    int dur = DataBase::get_DataBase()->get_all_equipment_data()[equipment_id]["duration"];
    std::string effect_name = DataBase::get_DataBase()->get_all_equipment_data()[equipment_id]["effect_name"];
    Potion* item = new Potion(item_id, equipment_id, equipment_class, item_characteristics, type, dur, effect_name);
    potions.insert(item);
    return item;
}

void Player::save_all_items(QFile &out)
{
    //размер контейнера items
    size_t size = items.size();
    //запись размера items
    out.write((char*)& size, sizeof(size));
    //цикл для записи каждого объекта класса Equipment
    for(const auto &i : items)
    {
        //вызов метода записи у объекта класса Equipment
        i->save(out);
    }

    size = potions.size();
    //запись размера potions
    out.write((char*)& size, sizeof(size));
    //цикл для записи каждого объекта класса potions
    for(const auto &i : potions)
    {
        //вызов метода записи у объекта класса potions
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

void Player::load_all_items(QFile &in)
{
    //переменная для размеров контейнеров
    size_t size;
    //чтение размера контейнера items
    in.read((char*)& size, sizeof(size));
    //очистка контейнера weaponary
    items.clear();
    //цикл для заполенения контейнера данными
    for(int i = 0; i < size; i++)
    {
        //создание нового объекта класса Weapon без значений
        Equipment* item = new Equipment();
        //вызов метода чтения объекта класса Weapon
        item->load(in);
        //добавление в контейнер указателя на объект класса Weapon
        items.insert(item);
    }

    //чтение размера контейнера potions
    in.read((char*)& size, sizeof(size));
    //очистка контейнера weaponary
    potions.clear();
    //цикл для заполенения контейнера данными
    for(int i = 0; i < size; i++)
    {
        //создание нового объекта класса Weapon без значений
        Potion* item = new Potion();
        //вызов метода чтения объекта класса Weapon
        item->load(in);
        //добавление в контейнер указателя на объект класса Weapon
        potions.insert(item);
    }

    //чтение размера контейнера active_effects
    in.read((char*)& size, sizeof(size));
    //очистка контейнера active_effects
    active_effects.clear();
    //цикл для заполения контейнера данными
    for(int i = 0; i < size; i++)
    {
        size_t eff_size;
        //чтение размера name
        in.read((char*)& eff_size, sizeof(eff_size));
        //присваивание name строки из пробелов длиной size-1, без /0
        std::string eff_name = std::string(eff_size - 1, ' ');
        //чтение name\e
        in.read(eff_name.data(), eff_size);
        //инициализация указателя новым объектом класса Effect


        Effect* item = new Effect(eff_name, this);


        //вызов метода чтения объекта класса Effect или его наследника
        item->load(in);
        //добавление в контейнер указателя на объект наследника класса Effect
        active_effects.push_back(item);
    }
}

void Player::load_equiped_items()
{
//    //цикл для заполнения контейнера equiped_weaponary
//    for(auto & i : items)
//    {
//        //проверка на экипированность предмета
//        if(i->get_equiped())
//        {
//            //присваивание указателя по ключу
//            equiped_weaponary[i->get_type()] = i;
//        }
//    }
}

void Player::save(QFile& out)
{
    //размер name
    size_t size = name.size() + 1;
    //запись размера name
    out.write((char*)& size, sizeof(size));
    //запись name
    out.write(name.c_str(), size);


    out.write((char*)& players_position, sizeof(players_position));


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

void Player::load(QFile &in)
{
    //переменная для размера строки и контейнера
    size_t size;
    //чтение размера строки
    in.read((char*) & size, sizeof(size));
    //присваивание name строки из пробелов длиной size-1, без /0
    name = std::string(size - 1, ' ');
    //чтение name
    in.read(name.data(), size);


    in.read((char*)& players_position, sizeof(players_position));


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

