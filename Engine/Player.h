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

    std::multiset <Equipment*, Equipment_Comparator> items;
    std::multiset <Potion*, Equipment_Comparator> potions;
    std::map<std::string, Equipment*> equipped_items;

    std::vector<Effect*> active_effects;

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

    std::multiset <Equipment*, Equipment_Comparator>* get_items();
    std::multiset <Potion*, Equipment_Comparator>* get_potions();


    std::vector<Effect*>* get_active_effects();

    std::map <std::string, Equipment*>* get_equipped_items();

    Equipment* add_item(const std::string& FileName);

    void attack(Player *pl);
    void use_potion(Potion*);
    void equip_item(Equipment* item, std::string place);
    void unequip_item(std::string place);
    void process_active_effects();


    int die();

    void save(QFile& out);
    void load(QFile& in);
};

#endif //PLAYER_H
