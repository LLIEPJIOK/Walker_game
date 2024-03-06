#ifndef PLAYER_H
#define PLAYER_H



#include "Engine/Transceiver.h"
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

    Coordinates::Hex<int> get_players_position() const;
    void set_players_position(const Coordinates::Hex<int> new_position);

    int get_killed_player();
    void set_killed_player(int id);
    // coordinates manipulations

    void update_chars();
    static void set_current_id(int id);


    std::map<std::string, int>& get_characteristics();

    std::multiset <Equipment*, Equipment_Comparator>* get_items();
    std::multiset <Potion*, Equipment_Comparator>* get_potions();


    std::vector<Effect*>* get_active_effects();

    std::map <std::string, Equipment*>* get_equipped_items();

    Equipment* add_item(const std::string& FileName);

    std::pair<int, bool> attack(Player *pl);
    void use_potion(Potion*);
    void use_potion(std::string potion_name);
    void equip_item(Equipment* item, std::string place);
    void equip_item(std::string place, std::string eq_name);
    void unequip_item(std::string place, bool send = true);
    void process_active_effects();


    int die();

    void save(QFile& out);
    void load(QFile& in);
};

#endif //PLAYER_H
