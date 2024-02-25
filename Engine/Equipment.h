#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <map>
#include <random>
#include <string>
#include <QFile>
#define stringify( name ) #name

class Equipment
{
protected:
    int item_id;
    std::string name;
    std::unordered_map<std::string, int> item_characteristics;
    std::string equipment_class;
    std::string type;
    bool is_equiped;
    bool primary_slot;
    std::random_device r;

    // нужно для фронта, сохранять и загружать не надо!
    bool is_front_equiped;


public:
    Equipment(std::string name);
    Equipment(int ID, std::string name, std::string equipment_class, std::unordered_map<std::string, int> characteristics, std::string type);
    Equipment() = default;
    std::string get_name() const;
    std::string get_type() const;
    std::string get_class() const;
    const std::unordered_map<std::string, int>* get_item_characteristics() const;
    int get_id() const;
    bool get_equiped() const;
    bool get_front_equiped() const;
    bool get_primary_equipped() const;
    void change_equiped();
    void change_front_equiped();
    void set_primary_equipped(bool val);

    virtual void save(QFile& out);
    virtual void load(QFile& in);

    virtual ~Equipment(){}
};



class Jewel : public Equipment {
private:
    void augment(std::string tier, std::string field, std::string choice);
public:
    Jewel(int ID, std::string name, std::string equipment_class, std::unordered_map<std::string, int> characteristics, std::string type);
    Jewel(std::string type, int turn_number);
    Jewel() = default;
};

class Weapon : public Equipment {
public:
    Weapon(int ID, std::string name, std::string equipment_class, std::unordered_map<std::string, int> characteristics, std::string type);
    Weapon() = default;
};

class Armour : public Equipment {
public:
    Armour(int ID, std::string name, std::string equipment_class, std::unordered_map<std::string, int> characteristics, std::string type);
    Armour() = default;
};

class Potion : public Equipment {
private:
    int duration;
    std::string effect_name;

public:
    Potion(int ID, std::string name, std::string equipment_class, std::unordered_map<std::string, int> characteristics, std::string type, int duration, std::string effect_name);
    Potion(std::string name);
    Potion() = default;

    void dec_duration();
    int get_duration() const;
    std::string get_effect_name() const;

    void save(QFile& out) override;
    void load(QFile &in) override;
};

class Equipment_Comparator {
public:
    bool operator ()(const Equipment* first, const Equipment* second) const { return first->get_name() < second->get_name(); }
    bool operator ()(const Equipment& first, const Equipment& second) const { return first.get_name() < second.get_name(); }
};

#endif
