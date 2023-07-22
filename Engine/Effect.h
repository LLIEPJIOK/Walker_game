#pragma once
#include "Player.h"

using V = void(Player* target, int counter);

class Effect
{
protected:
    Player* target; // не уверен, стоит ли вообще это хранить здесь...
	std::string effect_name;
	std::string effect_type;
    std::map<std::string, int> special_chs;
    int effect_counter; // у некоторых эффектов изменяется execute_effect в зависимости от количества "стаков" - counter
    int effect_duration;
    bool dispellable; // можно ли развеять эффект
    V* execute_effect_ptr; // указатель на функцию, которая будет какждый ход выполнять опр. действия

public:
    // constructors and destructor
    Effect();
    Effect(std::string _effect_name, Player* _target); // дефолтик
    Effect(std::string _effect_name, Player* _target, int dur); // с опр. длительностью
    Effect(std::string effect_name, Player* _target, int dur, int counter); // с опр. кол-вом стаков и длительностью
    ~Effect() = default;

    // getters
    std::map<std::string, int>& get_sp_chs();
    std::string get_effect_name() const;
    std::string get_effect_type() const;
    int get_effect_duration() const;
    int get_effect_counter() const;
    bool is_dispellable() const;

    // setters
	void set_effect_counter(int);
	void set_effect_duration(int);
    void set_special_chs(std::map<std::string, int>& chs);

    // incr & decr
	void dec_duration();
    void inc_duration();
	void inc_counter();
    void dec_counter();

    // наложение эффекта
    void apply_effect();

    // исполнение эффекта
    void execute_effect();

    // обращение эффекта
    void reverse_effect(Player& target);

    // методы сохранения и загрузки
    void save(std::ofstream& out);
    void load(std::ifstream& in);
};
