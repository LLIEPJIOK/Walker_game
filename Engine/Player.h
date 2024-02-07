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

class Effect;

class Player
{
private:
    static int CURRENT_ID;
    const int PLAYER_ID;
    std::string name;
    std::pair<int, int> previous_direction;
    int x, y;
    int killed_player;

    std::map<std::string, int> characteristics;

    std::multiset <Equipment*, Equipment_Comparator> weaponary;
    std::multiset <Equipment*, Equipment_Comparator> armourment;
    std::multiset <Equipment*, Equipment_Comparator> potions;
    std::multiset <Equipment*, Equipment_Comparator> jewellery;

    std::vector<Effect*> active_effects;
    std::map<std::string, Equipment*> equiped_weaponary;
    std::map<std::string, Equipment*> equiped_armourment;
    std::map<std::string, Equipment*> equiped_jewellery;

    void save_all_items(QFile& out);
    void load_all_items(QFile& in);
    void load_equiped_items();

public:
    Player(const std::string& _name);
    ~Player();

    int get_id() const;
    std::string get_name() const;


    int get_killed_player();
    void set_killed_player(int id);
    // coordinates manipulations
    int get_x() const;
    void set_x(const int& _x);
    int get_y() const;
    void set_y(const int& _y);
    std::pair<int, int> get_previous_direction() const;
    void set_previous_direction(const std::pair<int, int>& dir);
    void change_x(const bool& is_right);
    void change_y(const bool& is_up);
    void update_chars();
    static void set_current_id(int id);


    std::map<std::string, int>& get_characteristics();

    std::multiset <Equipment*, Equipment_Comparator>* get_weaponary();
    std::multiset <Equipment*, Equipment_Comparator>* get_armourment();
    std::multiset <Equipment*, Equipment_Comparator>* get_potions();
    std::multiset <Equipment*, Equipment_Comparator>* get_jewellery();

    std::vector<Effect*>* get_active_effects();

    std::map <std::string, Equipment*>* get_equiped_weaponary();
    std::map <std::string, Equipment*>* get_equiped_armourment();
    std::map <std::string, Equipment*>* get_equiped_jewellery();

    Equipment* add_item(const std::string& FileName);

    void attack(Player *pl);
    void use_potion(Potion*);
    void equip_item(std::map <std::string, Equipment*>* container, Equipment* item, std::string place);
    void unequip_item(Equipment* equipment, std::string place);
    void process_active_effects();


    int die();

    void save(QFile& out);
    void load(QFile& in);
};

#endif //PLAYER_H
