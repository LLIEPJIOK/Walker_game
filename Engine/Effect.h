#pragma once
#include "Player.h"
class Effect
{
protected:
	std::string effect_name;
	std::string effect_type;
    int effect_counter; // у некоторых эффектов изменяется execute_effect в зависимости от количества "стаков" - counter
	int effect_duration;
    bool dispellable = 1; // можно ли развеять эффект

public:
	Effect();
    virtual ~Effect(){};
	std::string get_effect_name();
	std::string get_effect_type();
	int get_effect_duration();
	int get_effect_counter();
	bool is_dispellable();
	void set_effect_counter(int);
	void set_effect_duration(int);
	void dec_duration();
	void inc_counter();
	void dec_counter();

    virtual void apply_effect(Player&, int) = 0; // наложение эффекта на выбранного игрока на выбранное количество ходов
    virtual void apply_effect(Player&, int, int) = 0; // наложение эффекта на выбранного игрока на выбранное количество ходов с выбранным количеством стаков
    virtual void execute_effect(Player&) = 0; // исполнение эффекта
    virtual void reverse_effect(Player&) = 0; // обращение временного эффекта, если тот временно изменял какой-то стат на стадии наложение эффекта
};

class Regeneration_effect : public Effect // регенерирует очки здоровья в процентном соотношении
{
	std::map<std::string, int> chars;
public:
    Regeneration_effect();
	void apply_effect(Player&, int);
	void apply_effect(Player&, int, int);
	void execute_effect(Player&);
	void reverse_effect(Player&);
};

class Burning_effect : public Effect // горение, потеря очков здоровья, зависит от текущей брони
{
	std::map<std::string, int> chars;
public:
	Burning_effect();
	void apply_effect(Player&, int);
	void apply_effect(Player&, int, int);
	void execute_effect(Player&);
	void reverse_effect(Player&);
};

class Shock_effect : public Effect // шок, потеря очков здоровья, временное уменьшение атаки на 4
{
	std::map<std::string, int> chars;
public:
	Shock_effect();
	void apply_effect(Player&, int);
	void apply_effect(Player&, int, int);
	void execute_effect(Player&);
	void reverse_effect(Player&);
};

class Intoxication_effect : public Effect // отравление, каждый ход наносит урон в 8% от текущего здоровья
{
public:
	Intoxication_effect();
	void apply_effect(Player&, int);
	void apply_effect(Player&, int, int);
	void execute_effect(Player&);
	void reverse_effect(Player&);
};

class Frostbite_effect : public Effect // обморожение, уменьшает броню на 5, модификатор броска на 1, каждый ход наносит урон, равный количеству стаков (<3)
{
public:
	Frostbite_effect();
	void apply_effect(Player&, int);
	void apply_effect(Player&, int, int);
	void execute_effect(Player&);
	void reverse_effect(Player&);
};

class Bleeding_effect : public Effect // кровотечение, наносит урон каждый ход, зависящий от пройденного расстояния
{
	int latest_x, latest_y;
public:
	Bleeding_effect();
	void apply_effect(Player&, int);
	void apply_effect(Player&, int, int);
	void execute_effect(Player&);
	void reverse_effect(Player&);
};

class Slowdown_effect : public Effect // замедление, понижает ловкость на 2, понижает модификатор броска на 2
{
public:
    Slowdown_effect();
    void apply_effect(Player&, int);
    void apply_effect(Player&, int, int);
    void execute_effect(Player&);
    void reverse_effect(Player&);
};

class Haste_effect : public Effect // ускорение, повышает ловкость на 2, увеличивает модификатор броска на 2
{
public:
    Haste_effect();
    void apply_effect(Player&, int);
    void apply_effect(Player&, int, int);
    void execute_effect(Player&);
    void reverse_effect(Player&);
};

class Endurance_effect : public Effect // стойкость, временно повышает броню на 30%, текущее снижение урона на 20%, увеличение здоровья на 20 (процент от значений во время наложения эффекта)
{
private:
    int extra_armour, extra_pierce_armour;
public:
    Endurance_effect(int, int);
    void apply_effect(Player&, int);
    void apply_effect(Player&, int, int);
    void execute_effect(Player&);
    void reverse_effect(Player&);
};

class Empower_effect : public Effect // усиление, временно повышает атаку на 20%, прорубающий урон на 10%, шанс крита на 10%, крит. удар на 10% (процент от значений во время наложения эффекта)
{
private:
    int extra_atk, extra_pierce, extra_ctir_ch, extra_crit;
public:
    Empower_effect(int, int, int, int);
    void apply_effect(Player&, int);
    void apply_effect(Player&, int, int);
    void execute_effect(Player&);
    void reverse_effect(Player&);
};

class Dispell_effect : public Effect // развеивает все активные эффекты, которые можно развеять
{
public:
    Dispell_effect();
    void apply_effect(Player&, int);
    void apply_effect(Player&, int, int);
    void execute_effect(Player&);
    void reverse_effect(Player&);
};


class All_effects
{
	
private:
	std::map<std::string, Effect*> effects;
	static All_effects* effects_data;
public:

	~All_effects();
	All_effects();
	All_effects(const All_effects&) = delete;
	const All_effects& operator = (const All_effects&) = delete;
	std::map<std::string, Effect* >* get_effects();
	static All_effects* get_effects_data();

};
