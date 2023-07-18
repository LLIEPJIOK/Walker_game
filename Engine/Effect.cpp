#include "Effect.h"
#include <vector>
All_effects* All_effects::effects_data = 0;

Effect::Effect()
{
	return;
}

std::string Effect::get_effect_name()
{
	return effect_name;
}

std::string Effect::get_effect_type()
{
	return effect_type;
}

int Effect::get_effect_duration()
{
	return effect_duration;
}

int Effect::get_effect_counter()
{
    return effect_counter;
}

std::map<std::string, int> &Effect::get_sp_chs()
{
    return special_chs;
}

bool Effect::is_dispellable()
{
	return dispellable;
}

void Effect::set_effect_counter(int value)
{
	effect_counter = value;
}

void Effect::set_effect_duration(int value)
{
    effect_duration = value;
}

void Effect::set_special_chs(std::map<std::string, int> &chs)
{
    special_chs = chs;
}

void Effect::dec_duration()
{
	effect_duration--;
}

void Effect::inc_counter()
{
	effect_counter++;
}

void Effect::dec_counter()
{
    effect_counter--;
}

//метод для чтения имени, т.к. эффект явялется абстрактным классом
std::string Effect::read_name(std::ifstream &in)
{
    //переменная для размера строк
    size_t size;
    //чтение размера name
    in.read((char*)& size, sizeof(size));
    //присваивание namr строки из пробелов длиной size-1, без /0
    std::string name = std::string(size - 1, ' ');
    //чтение namr\e
    in.read(name.data(), size);

    return name;
}

void Effect::save(std::ofstream& out)
{
    //размер effect_name с /0
    size_t size = effect_name.size() + 1;
    //запись размера effect_name
    out.write((char*)& size, sizeof(size));
    //запись effect_name
    out.write(effect_name.c_str(), size);

    //размер effect_type c /0
    size = effect_type.size() + 1;
    //запись размера effect_type
    out.write((char*)& size, sizeof(size));
    //запись effect_type
    out.write(effect_type.c_str(), size);

    //запись effect_counter
    out.write((char*)& effect_counter, sizeof(effect_counter));

    //запись effect_duration
    out.write((char*)& effect_duration, sizeof(effect_duration));

    //запись dispallable
    out.write((char*)& dispellable, sizeof(dispellable));
}

void Effect::load(std::ifstream& in)
{
    //переменная для рамзера строк
    size_t size;
    //чтение размера effect_type
    in.read((char*)& size, sizeof(size));
    //присваивание effect_type строки из пробелов длиной size-1, без /0
    effect_type = std::string(size - 1, ' ');
    //чтение effect_type данных размера size
    in.read(effect_type.data(), size);

    //чтение effect_counter
    in.read((char*)& effect_counter, sizeof(effect_counter));

    //чтение effect_duration
    in.read((char*)& effect_duration, sizeof(effect_duration));

    //чтение dispallable
    in.read((char*)& dispellable, sizeof(dispellable));
}

// ВНИМАНИЕ! ДАЛЬШЕ НАСРАНО

Regeneration_effect::Regeneration_effect()
{
    effect_name = "регенерация";
    effect_type = "положительный";
	effect_duration = 3;
	effect_counter = 1;
    dispellable = 1;
    special_chs = std::map<std::string, int>();
}

void Regeneration_effect::apply_effect(Player& target, int duration)
{
	for (const auto& i : *target.get_active_effects())
		if (i->get_effect_name() == effect_name)
		{
			i->set_effect_duration(duration);
			i->inc_counter();
			return;
        }
    Regeneration_effect* tmp = new Regeneration_effect();
	tmp->set_effect_duration(duration);
	target.get_active_effects()->push_back(tmp);
}

void Regeneration_effect::apply_effect(Player& target, int duration, int counter)
{
	for (const auto& i : *target.get_active_effects())
		if (i->get_effect_name() == effect_name)
		{
			i->set_effect_duration(duration);
			i->inc_counter();
			return;
        }
    Regeneration_effect* tmp = new Regeneration_effect();
	tmp->set_effect_duration(duration);
	tmp->set_effect_counter(counter);
	target.get_active_effects()->push_back(tmp);
}

void Regeneration_effect::execute_effect(Player& target)
{
	int Max_HP = target.get_characteristics().at("MAX_HP");
	int HP = target.get_characteristics().at("HP");
	int dif = Max_HP - HP;
	
	if (dif > 0)
	{
		if (dif > 0.2 * Max_HP)
            target.get_characteristics().at("HP") += dif * 0.2;
        else if (dif > 1)
			target.get_characteristics().at("HP") += 2;
        else
            target.get_characteristics().at("HP") = Max_HP;
    }
}

void Regeneration_effect::reverse_effect(Player&)
{
	return;
}

Burning_effect::Burning_effect()
{
    effect_name = "горение";
    effect_type = "отрицательный";
	effect_duration = 3;
	effect_counter = 1;
    dispellable = 1;
}

void Burning_effect::apply_effect(Player& target, int duration)
{
	for (const auto& i : *target.get_active_effects())
		if (i->get_effect_name() == effect_name)
		{
			i->set_effect_duration(duration);
			i->inc_counter();
			return;
		}

    Burning_effect* tmp = new Burning_effect();
    tmp->set_effect_duration(duration);
	target.get_active_effects()->push_back(tmp);
}

void Burning_effect::apply_effect(Player& target, int duration, int counter)
{
	for (const auto& i : *target.get_active_effects())
		if (i->get_effect_name() == effect_name)
		{
			i->set_effect_duration(duration);
			i->inc_counter();
			return;
		}

	Burning_effect* tmp = new Burning_effect();
	tmp->set_effect_duration(duration);
	tmp->set_effect_counter(counter);
	target.get_active_effects()->push_back(tmp);
}

void Burning_effect::execute_effect(Player& target)
{
    int HP = target.get_characteristics().at("HP");
    int dmg = HP * (double)(0.02 * (double)effect_counter); // надо проверить как работает формулка...
	if (effect_counter > 2)
        dmg += target.get_characteristics().at("ARM") * 0.2;

    target.get_characteristics().at("HP") -= dmg;
}

void Burning_effect::reverse_effect(Player&)
{
	return;
}

Shock_effect::Shock_effect()
{
    effect_name = "шок";
    effect_type = "отрицательный";
	effect_duration = 2;
	effect_counter = 1;
    dispellable = 1;
    special_chs = std::map<std::string, int>();
}

void Shock_effect::apply_effect(Player& target, int duration)
{
	for (const auto& i : *target.get_active_effects())
		if (i->get_effect_name() == effect_name)
		{
			i->set_effect_duration(duration);
			i->inc_counter(); 
			return;
        }

    Shock_effect* tmp = new Shock_effect();
    tmp->set_effect_duration(duration);
    int decr_ch = 25;
    tmp->get_sp_chs().emplace("CRIT_CH_FLAT", -1 * decr_ch);
    target.get_characteristics().at("CRIT_CH_FLAT") -= decr_ch;
    target.get_active_effects()->push_back(tmp);
}

void Shock_effect::apply_effect(Player& target, int duration, int counter)
{
	for (const auto& i : *target.get_active_effects())
		if (i->get_effect_name() == effect_name)
		{
			i->set_effect_duration(duration);
			i->inc_counter();
			return;
        }

    Shock_effect* tmp = new Shock_effect();
    tmp->set_effect_duration(duration);
    int decr_ch = 25;
    tmp->get_sp_chs().emplace("CRIT_CH_FLAT", -1 * decr_ch);
    target.get_characteristics().at("CRIT_CH_FLAT") -= decr_ch;
    target.get_active_effects()->push_back(tmp);
}

void Shock_effect::execute_effect(Player& target)
{
    target.get_characteristics().at("HP") -= 5;
}

void Shock_effect::reverse_effect(Player& target)
{
	target.get_characteristics().at("ATK") += 4;
}

Intoxication_effect::Intoxication_effect()
{
    effect_name = "отравление";
    effect_type = "отрицательный";
	effect_duration = 3;
	effect_counter = 1;
	dispellable = 1;
    special_chs = std::map<std::string, int>();
}

void Intoxication_effect::apply_effect(Player& target, int duration)
{
	for (const auto& i : *target.get_active_effects())
		if (i->get_effect_name() == effect_name)
		{
			i->set_effect_duration(duration);
			i->inc_counter();
			return;
		}

    Intoxication_effect* tmp = new Intoxication_effect();
    tmp->set_effect_duration(duration);
    int decr_atk = target.get_characteristics().at("ATK") * 0.25;
    tmp->get_sp_chs().emplace("ATK", -1 * decr_atk);
    target.get_characteristics().at("ATK") -= decr_atk;
    target.get_active_effects()->push_back(tmp);
}

void Intoxication_effect::apply_effect(Player& target, int duration, int counter)
{
	for (const auto& i : *target.get_active_effects())
		if (i->get_effect_name() == effect_name)
		{
			i->set_effect_duration(duration);
			i->inc_counter();
			return;
		}

    Intoxication_effect* tmp = new Intoxication_effect();
    tmp->set_effect_duration(duration);
    int decr_atk = target.get_characteristics().at("ATK") * 0.25;
    tmp->get_sp_chs().emplace("ATK", -1 * decr_atk);
    target.get_characteristics().at("ATK") -= decr_atk;
    target.get_active_effects()->push_back(tmp);
}

void Intoxication_effect::execute_effect(Player& target)
{
	target.get_characteristics().at("HP") -= 0.08 * target.get_characteristics().at("HP");
}

void Intoxication_effect::reverse_effect(Player& target)
{
    target.get_characteristics().at("ATK") -= special_chs.at("ATK");
}

Frostbite_effect::Frostbite_effect()
{
    effect_name = "обморожение";
    effect_type = "отрицательный";
	effect_duration = 3;
	effect_counter = 1;
	dispellable = 1;
    special_chs = std::map<std::string, int>();
}

void Frostbite_effect::apply_effect(Player& target, int duration)
{
	for (const auto& i : *target.get_active_effects())
		if (i->get_effect_name() == effect_name)
		{
			i->set_effect_duration(duration);
			i->inc_counter();
			return;
		}
	Frostbite_effect* tmp = new Frostbite_effect();
	tmp->set_effect_duration(duration);
	target.get_active_effects()->push_back(tmp);
    target.get_characteristics().at("ROLL_MOD") -= 1;
	target.get_characteristics().at("ARM") -= 5;
}

void Frostbite_effect::apply_effect(Player& target, int duration, int counter)
{
	for (const auto& i : *target.get_active_effects())
		if (i->get_effect_name() == effect_name)
		{
			i->set_effect_duration(duration);
			i->inc_counter();
			return;
		}
	Frostbite_effect* tmp = new Frostbite_effect();
	tmp->set_effect_duration(duration);
	tmp->set_effect_counter(counter);
	target.get_active_effects()->push_back(tmp);
    target.get_characteristics().at("ROLL_MOD") -= 1;
	target.get_characteristics().at("ARM") -= 5;
}

void Frostbite_effect::execute_effect(Player& target)
{
	target.get_characteristics().at("HP") -= effect_counter;
	if (effect_counter < 3)
		effect_counter++;
}

void Frostbite_effect::reverse_effect(Player& target)
{
    target.get_characteristics().at("ROLL_MOD") += 1;
	target.get_characteristics().at("ARM") += 5;
}

// BLEEDING ТРЕБУЕТ РЕВОРКА

Bleeding_effect::Bleeding_effect()
{
    effect_name = "кровотечение";
    effect_type = "негативный";
	effect_duration = 3;
	effect_counter = 1;
	dispellable = 0;
}

void Bleeding_effect::apply_effect(Player& target, int duration)
{
	for (const auto& i : *target.get_active_effects())
		if (i->get_effect_name() == effect_name)
		{
			i->set_effect_duration(duration);
			i->inc_counter();
			return;
        }
	Bleeding_effect* tmp = new Bleeding_effect();
	tmp->set_effect_duration(duration);
	target.get_active_effects()->push_back(tmp);
}

void Bleeding_effect::apply_effect(Player& target, int duration, int counter)
{
	for (const auto& i : *target.get_active_effects())
		if (i->get_effect_name() == effect_name)
		{
			i->set_effect_duration(duration);
			i->inc_counter();
			return;
        }
	Bleeding_effect* tmp = new Bleeding_effect();
	tmp->set_effect_duration(duration);
	tmp->set_effect_counter(counter);
	target.get_active_effects()->push_back(tmp);
}

void Bleeding_effect::execute_effect(Player& target)
{
    //int dif = sqrt((latest_x - target.get_x()) * (latest_x - target.get_x()) + (latest_y - target.get_y()) * (latest_y - target.get_y())) * sqrt(effect_counter);
    //target.get_characteristics().at("HP") -= dif;
}

void Bleeding_effect::reverse_effect(Player&)
{
	return;
}

Slowdown_effect::Slowdown_effect()
{
    effect_name = "замедление";
    effect_type = "отрицательный";
    effect_duration = 3;
    effect_counter = 1;
    dispellable = 0;
}

void Slowdown_effect::apply_effect(Player &target, int duration)
{
    for (const auto& i : *target.get_active_effects())
        if (i->get_effect_name() == effect_name)
        {
            i->set_effect_duration(duration);
            i->inc_counter();
            return;
        }
    Slowdown_effect* tmp = new Slowdown_effect();
    tmp->set_effect_duration(duration);
    target.get_active_effects()->push_back(tmp);
}

void Slowdown_effect::apply_effect(Player &target, int duration, int counter)
{
    for (const auto& i : *target.get_active_effects())
        if (i->get_effect_name() == effect_name)
        {
            i->set_effect_duration(duration);
            i->inc_counter();
            return;
        }
    Slowdown_effect* tmp = new Slowdown_effect();
    tmp->set_effect_duration(duration);
    tmp->set_effect_counter(counter);
    target.get_characteristics().at("AGIL") -= 2;
    target.get_characteristics().at("ROLL_MOD") -= 2;
    target.get_active_effects()->push_back(tmp);
}

void Slowdown_effect::execute_effect(Player &)
{
    return;
}

void Slowdown_effect::reverse_effect(Player & target)
{
    target.get_characteristics().at("AGIL") += 2;
    target.get_characteristics().at("ROLL_MOD") += 2;
}

Haste_effect::Haste_effect()
{
    effect_name = "ускорение";
    effect_type = "положительный";
    effect_duration = 3;
    effect_counter = 1;
    dispellable = 0;
}

void Haste_effect::apply_effect(Player &target, int duration)
{
    for (const auto& i : *target.get_active_effects())
        if (i->get_effect_name() == effect_name)
        {
            i->set_effect_duration(duration);
            i->inc_counter();
            target.get_characteristics().at("AGIL") += 2;
            target.get_characteristics().at("ROLL_MOD") += 2;
            return;
        }
    Haste_effect* tmp = new Haste_effect();
    tmp->set_effect_duration(duration);
    target.get_characteristics().at("AGIL") += 2;
    target.get_characteristics().at("ROLL_MOD") += 2;
    target.get_active_effects()->push_back(tmp);
}

void Haste_effect::apply_effect(Player &target, int duration, int counter)
{
    for (const auto& i : *target.get_active_effects())
        if (i->get_effect_name() == effect_name)
        {
            i->set_effect_duration(duration);
            i->inc_counter();
            target.get_characteristics().at("AGIL") += 2;
            target.get_characteristics().at("ROLL_MOD") += 2;
            return;
        }
    Haste_effect* tmp = new Haste_effect();
    tmp->set_effect_duration(duration);
    tmp->set_effect_counter(counter);
    target.get_characteristics().at("AGIL") += 2;
    target.get_characteristics().at("ROLL") += 2;
    target.get_active_effects()->push_back(tmp);
}

void Haste_effect::execute_effect(Player &)
{
    return;
}

void Haste_effect::reverse_effect(Player & target)
{
    target.get_characteristics().at("AGIL") -= 2 * effect_counter;
    target.get_characteristics().at("ROLL_MOD") -= 2 * effect_counter;
}

Endurance_effect::Endurance_effect(int armour, int pierce)
{
    effect_name = "стойкость";
    effect_type = "положительный";
    effect_duration = 3;
    effect_counter = 1;
    dispellable = 0;
    special_chs.emplace("ARM", armour);
    special_chs.emplace("PIERCE_ARM", pierce);
}

void Endurance_effect::apply_effect(Player &target, int duration)
{
    for (const auto& i : *target.get_active_effects())
        if (i->get_effect_name() == effect_name)
        {
            i->set_effect_duration(duration);
            i->inc_counter();
            return;
        }
    int arm = target.get_characteristics().at("ARM") * 0.3;
    int pierce = target.get_characteristics().at("PIERCE_ARM") * 0.2;
    target.get_characteristics().at("PIERCE_ARM") += pierce;
    target.get_characteristics().at("ARM") += arm;
    target.get_characteristics().at("HP") += 20;
    Endurance_effect* tmp = new Endurance_effect(arm, pierce); // в конструкторе эффекта сохраняется бафф армора и бафф снижения урона
    tmp->set_effect_duration(duration);
    target.get_active_effects()->push_back(tmp);
}

void Endurance_effect::apply_effect(Player &target, int duration, int counter)
{
    for (const auto& i : *target.get_active_effects())
        if (i->get_effect_name() == effect_name)
        {
            i->set_effect_duration(duration);
            i->inc_counter();
            return;
        }
    int arm = target.get_characteristics().at("ARM") * 0.3;
    int pierce = target.get_characteristics().at("PIERCE_ARM") * 0.2;
    target.get_characteristics().at("PIERCE_ARM") += pierce;
    target.get_characteristics().at("ARM") += arm;
    target.get_characteristics().at("HP") += 20;
    Endurance_effect* tmp = new Endurance_effect(arm, pierce); // в конструкторе эффекта сохраняется бафф армора и бафф снижения урона
    tmp->set_effect_duration(duration);
    tmp->set_effect_counter(counter);
    target.get_active_effects()->push_back(tmp);
}

void Endurance_effect::execute_effect(Player &)
{
    return;
}

void Endurance_effect::reverse_effect(Player & target)
{
    target.get_characteristics().at("PIERCE_ARM") -= special_chs["PIERCE_ARM"];
    target.get_characteristics().at("ARM") -= special_chs["ARM"];
    if(target.get_characteristics().at("HP") <= 20)
        target.get_characteristics().at("HP") = 1;
    else
        target.get_characteristics().at("HP") -= 20;
}

Empower_effect::Empower_effect(int atk, int pierce, int crit_ch, int crit)
{
    effect_name = "усиление";
    effect_type = "положительный";
    effect_duration = 3;
    effect_counter = 1;
    dispellable = 0;
    special_chs.emplace("ATK", atk);
    special_chs.emplace("PIERCE", pierce);
    special_chs.emplace("CRIT_CH", crit_ch);
    special_chs.emplace("CRIT_DMG", crit);
}

void Empower_effect::apply_effect(Player &target, int duration)
{
    for (const auto& i : *target.get_active_effects())
        if (i->get_effect_name() == effect_name)
        {
            i->set_effect_duration(duration);
            i->inc_counter();
            return;
        }
    int atk = target.get_characteristics().at("ATK") * 0.2;
    int pierce = target.get_characteristics().at("PIERCE") * 0.1;
    int ch = target.get_characteristics().at("CRIT_CH") * 0.1;
    int dmg = target.get_characteristics().at("CRIT_DMG") * 0.1;
    target.get_characteristics().at("PIERCE") += pierce;
    target.get_characteristics().at("ATK") += atk;
    target.get_characteristics().at("CRIT_CH") += ch;
    target.get_characteristics().at("CRIT_DMG") += dmg;
    Empower_effect* tmp = new Empower_effect(atk, pierce, ch, dmg);
    tmp->set_effect_duration(duration);
    target.get_active_effects()->push_back(tmp);
}

void Empower_effect::apply_effect(Player &target, int duration, int counter)
{
    for (const auto& i : *target.get_active_effects())
        if (i->get_effect_name() == effect_name)
        {
            i->set_effect_duration(duration);
            i->inc_counter();
            return;
        }
    int atk = target.get_characteristics().at("ATK") * 0.2;
    int pierce = target.get_characteristics().at("PIERCE") * 0.1;
    int ch = target.get_characteristics().at("CRIT_CH") * 0.1;
    int dmg = target.get_characteristics().at("CRIT_DMG") * 0.1;
    target.get_characteristics().at("PIERCE") += pierce;
    target.get_characteristics().at("ATK") += atk;
    target.get_characteristics().at("CRIT_CH") += ch;
    target.get_characteristics().at("CRIT_DMG") += dmg;
    Empower_effect* tmp = new Empower_effect(atk, pierce, ch, dmg);
    tmp->set_effect_duration(duration);
    tmp->set_effect_counter(counter);
    target.get_active_effects()->push_back(tmp);
}

void Empower_effect::execute_effect(Player &)
{
    return;
}

void Empower_effect::reverse_effect(Player & target)
{
    target.get_characteristics().at("PIERCE") -= special_chs["PIERCE"];
    target.get_characteristics().at("ATK") -= special_chs["ATK"];
    target.get_characteristics().at("CRIT_CH") -= special_chs["CRIT_CH"];
    target.get_characteristics().at("CRIT_DMG") -= special_chs["CRIT_DMG"];
}

Dispell_effect::Dispell_effect()
{
    effect_name = "развеивание";
    effect_type = "нейтральный";
    effect_duration = 1;
    effect_counter = 0;
    dispellable = 0;
}

void Dispell_effect::apply_effect(Player & target, int)
{

    for (auto i = target.get_active_effects()->begin(); i!=target.get_active_effects()->end(); i++)
        if ((*i)->is_dispellable())
        {
            (*i)->reverse_effect(target);
            delete* i;
            target.get_active_effects()->erase(i);
        }
}

void Dispell_effect::apply_effect(Player &target, int, int)
{
    for (auto i = target.get_active_effects()->begin(); i!=target.get_active_effects()->end(); i++)
        if ((*i)->is_dispellable())
        {
            (*i)->reverse_effect(target);
            delete* i;
            target.get_active_effects()->erase(i);
        }
}

void Dispell_effect::execute_effect(Player &)
{

}

void Dispell_effect::reverse_effect(Player &)
{

}

All_effects::~All_effects()
{
    for (const auto& i : effects)
        delete i.second;

}

All_effects::All_effects()
{
    effects.emplace(std::make_pair("регенерация", new Regeneration_effect()));
    effects.emplace(std::make_pair("горение", new Burning_effect()));
    effects.emplace(std::make_pair("шок", new Shock_effect()));
    effects.emplace(std::make_pair("отравление", new Intoxication_effect()));
    effects.emplace(std::make_pair("обморожение", new Frostbite_effect()));
    effects.emplace(std::make_pair("кровотечение", new Bleeding_effect()));
    effects.emplace(std::make_pair("замедление", new Slowdown_effect()));
    effects.emplace(std::make_pair("ускорение", new Haste_effect()));
    effects.emplace(std::make_pair("стойкость", new Endurance_effect(0, 0)));
    effects.emplace(std::make_pair("усиление", new Empower_effect(0,0,0,0)));
    effects.emplace(std::make_pair("развеивание", new Dispell_effect()));
}

std::map<std::string, Effect*>* All_effects::get_effects()
{
    return &effects;
}

All_effects* All_effects::get_effects_data()
{
    if (!effects_data)
        effects_data = new All_effects;
    return effects_data;
}
