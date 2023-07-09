
#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <map>
#include <string>
#include <fstream>


class Equipment
{
protected:
    int item_id;
    std::string name;
    std::map<std::string, int> item_characteristics;
    std::string equipment_class;
    std::string type;
    bool is_equiped;
public:
    Equipment(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type);
    Equipment();
    std::string get_name() const;
    std::string get_type() const;
    std::string get_class() const;
    const std::map<std::string, int>* get_item_characteristics() const;
    int get_id() const;
    bool get_equiped() const;
    void change_equiped();
    virtual ~Equipment(){}
};



class Jewel : public Equipment {
private:
    void make_ring(int turn_number);
    void make_necklace(int turn_number);
    void make_belt(int turn_number);
public:
    Jewel(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type);
    Jewel(std::string type, int turn_number);
};

class Weapon : public Equipment {
public:
    Weapon(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type);
};

class Armour : public Equipment {
public:
    Armour(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type);
};

class Potion : public Equipment {
private:
    int duration;
    std::string effect_name;
public:
    Potion(int ID, std::string name, std::string equipment_class, std::map<std::string, int> characteristics, std::string type, int duration, std::string effect_name);
    void dec_duration();
    int get_duration() const;
    std::string get_effect_name() const;
};

class Equipment_Comparator {
public:
    bool operator ()(const Equipment* first, const Equipment* second) const { return first->get_name() < second->get_name(); }
};

#endif
