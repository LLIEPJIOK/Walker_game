#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <set>
#include <map>
#include <set>
#include <cmath>
#include <vector>
#include <cassert>

#include "Equipment.h"
#include "Map/Hex.h"

class Effect;

class Player
{
private:
    static int CURRENT_ID;
    const int PLAYER_ID;
    std::string name;

    Coordinates::Hex<int> players_position;

    int killed_player;

    std::map<std::string, int> characteristics;

    std::multiset <Weapon*, Equipment_Comparator> weaponary;
    std::multiset <Armour*, Equipment_Comparator> armourment;
    std::multiset <Potion*, Equipment_Comparator> potions;
    std::multiset <Jewel*, Equipment_Comparator> jewellery;

    std::vector<Effect*> active_effects;
    std::map<std::string, Weapon*> equiped_weaponary;
    std::map<std::string, Armour*> equiped_armourment;
    std::map<std::string, Jewel*> equiped_jewellery;

    void save_all_items(QFile& out);
    void load_all_items(QFile& in);
    void load_equiped_items();

public:
    Player(const std::string& _name);
    ~Player();

    int get_id() const;
    std::string get_name() const;

    Coordinates::Hex<int> get_players_position() const;
    void set_players_position(const Coordinates::Hex<int> new_position);

    int get_killed_player();
    void set_killed_player(int id);
    // coordinates manipulations

    void update_chars();
    static void set_current_id(int id);


    std::map<std::string, int>& get_characteristics();

    std::multiset <Weapon*, Equipment_Comparator>* get_weaponary();
    std::multiset <Armour*, Equipment_Comparator>* get_armourment();
    std::multiset <Potion*, Equipment_Comparator>* get_potions();
    std::multiset <Jewel*, Equipment_Comparator>* get_jewellery();

    std::vector<Effect*>* get_active_effects();

    std::map <std::string, Weapon*>* get_equiped_weaponary();
    std::map <std::string, Armour*>* get_equiped_armourment();
    std::map <std::string, Jewel*>* get_equiped_jewellery();

    Equipment* add_item(const std::string& FileName);

    void attack(Player *pl);
    void use_potion(Potion*);

    void equip_weapon(Weapon* weapon, std::string place);
    void equip_armour(Armour* armour, std::string place);
    void equip_jewel(Jewel* jewel, std::string place);
    void unequip_item(Equipment* equipment, std::string place);
    void process_active_effects();


    int die();

    void save(QFile& out);
    void load(QFile& in);
};

#endif //PLAYER_H
