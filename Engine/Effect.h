#pragma once
#include "Player.h"
class Effect
{
protected:
	std::string effect_name;
	std::string effect_type;
	int effect_counter;
	int effect_duration;
	bool dispellable = 1;

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

	virtual void apply_effect(Player&, int) = 0;
	virtual void apply_effect(Player&, int, int) = 0;
	virtual void execute_effect(Player&) = 0;
	virtual void reverse_effect(Player&) = 0;
};

class Small_healing_effect : public Effect
{
	std::map<std::string, int> chars;
public:
	Small_healing_effect();
	void apply_effect(Player&, int);
	void apply_effect(Player&, int, int);
	void execute_effect(Player&);
	void reverse_effect(Player&);
};

class Burning_effect : public Effect
{
	std::map<std::string, int> chars;
public:
	Burning_effect();
	void apply_effect(Player&, int);
	void apply_effect(Player&, int, int);
	void execute_effect(Player&);
	void reverse_effect(Player&);
};

class Shock_effect : public Effect
{
	std::map<std::string, int> chars;
public:
	Shock_effect();
	void apply_effect(Player&, int);
	void apply_effect(Player&, int, int);
	void execute_effect(Player&);
	void reverse_effect(Player&);
};

class Intoxication_effect : public Effect
{
public:
	Intoxication_effect();
	void apply_effect(Player&, int);
	void apply_effect(Player&, int, int);
	void execute_effect(Player&);
	void reverse_effect(Player&);
};

class Frostbite_effect : public Effect
{
public:
	Frostbite_effect();
	void apply_effect(Player&, int);
	void apply_effect(Player&, int, int);
	void execute_effect(Player&);
	void reverse_effect(Player&);
};

class Bleeding_effect : public Effect
{
	int latest_x, latest_y;
public:
	Bleeding_effect();
	void apply_effect(Player&, int);
	void apply_effect(Player&, int, int);
	void execute_effect(Player&);
	void reverse_effect(Player&);
};

class Slowdown_effect : public Effect
{
public:
    Slowdown_effect();
    void apply_effect(Player&, int);
    void apply_effect(Player&, int, int);
    void execute_effect(Player&);
    void reverse_effect(Player&);
};

class Haste_effect : public Effect
{
public:
    Haste_effect();
    void apply_effect(Player&, int);
    void apply_effect(Player&, int, int);
    void execute_effect(Player&);
    void reverse_effect(Player&);
};

class Endurance_effect : public Effect
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

class Empower_effect : public Effect
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

class Dispell_effect : public Effect
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
