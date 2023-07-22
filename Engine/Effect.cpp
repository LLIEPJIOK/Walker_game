#include "Effect.h"
#include "Turn.h"
#include "DataBase.h"
#include <vector>

#define eff_data DataBase::get_DataBase()->get_all_effects_data()

void experiment_exe (Player* target, int counter)
{
    if (target == nullptr)
        throw std::invalid_argument("experiment_exe -- target = nullptr!");
    target ->get_characteristics().at("HP") -= 99 - counter;
}

// регенерация, восстанавливает 20% от недостающих ОЗ
void regeneration_exe (Player* target, int)
{
    if (target == nullptr)
        throw std::invalid_argument("regen_exe -- target = nullptr!");

    int max_hp = target->get_characteristics().at("MAX_HP");
    int cur_hp = target->get_characteristics().at("HP");
    int potential_heal = (int)((max_hp - cur_hp) * 0.2);
    if (potential_heal == 0 && (max_hp - cur_hp) != 0)
        target->get_characteristics().at("HP") += 1;
    else
        target->get_characteristics().at("HP") += potential_heal;
}

// горение, сначала наносит 8 урона в ход, псоле 2-х стаков наносит урон, зависимый от брони
void burning_exe (Player* target, int counter)
{
    if (target == nullptr)
        throw std::invalid_argument("burning_exe -- target = nullptr!");

    int base_dmg = 8;
    int dmg_multi = target->get_characteristics().at("ARM_FLAT") + 100;
    int heat_dmg = (int)((base_dmg * dmg_multi) / 100);
    if (counter > 2)
    {
        target->get_characteristics().at("HP") -= heat_dmg;
        return;
    }

    target->get_characteristics().at("HP") -= base_dmg;
}

// отравление, отнимает 10% текущих ОЗ за ход, немного зависит от числа стаков
void intoxication_exe (Player* target, int counter)
{
    if (target == nullptr)
        throw std::invalid_argument("intoxication_exe -- target = nullptr!");

    int cur_hp = target->get_characteristics().at("HP");
    int dmg = (int)((cur_hp + counter * 5) * 0.1);
    if (dmg == 0)
        dmg = 1;

    target->get_characteristics().at("HP") -= dmg;
}

// кровотечение, урон зависит от пройденного расстояния
void bleeding_exe (Player* target, int counter)
{
    if (target == nullptr)
        throw std::invalid_argument("bleeding_exe -- target = nullptr!");

    if (counter == 1)
        return;

    target->get_characteristics().at("HP") -= Turn::get_Turn()->get_rolled() * counter;
}

// обморожение, -2 к модификатору броска во время движения, понижает крит. шанс, наносит 5 урона ежеходно
void frostbite_exe (Player* target, int counter)
{
    if (target == nullptr)
        throw std::invalid_argument("frostbite_exe -- target = nullptr!");

    target->get_characteristics().at("HP") -= counter + 5;
}

// шок, значительно понижает хар-ки критов, наносит 5 урона ежеходно
void shock_exe (Player* target, int counter)
{
    if (target == nullptr)
        throw std::invalid_argument("shock_exe -- target = nullptr!");

    target->get_characteristics().at("HP") -= counter + 5;
}

// слабость, понижает атакующие хар-ки
void weakness_exe (Player* target, int) // слабость не имеет ежеходного действия
{
    if (target == nullptr)
        throw std::invalid_argument("weakness_exe -- target = nullptr!");
}

// усиление, повышает атакующие хар-ки
void empower_exe (Player* target, int) // усиление не имеет ежеходного действия
{
    if (target == nullptr)
        throw std::invalid_argument("empower_exe -- target = nullptr!");
}

// стойкость, повышает броню и макс. ОЗ
void endurance_exe (Player* target, int) // стойкость не имеет ежеходного действия
{
    if (target == nullptr)
        throw std::invalid_argument("endurance_exe -- target = nullptr!");
}

// ускорение, +2 к мод. броска движения
void haste_exe (Player* target, int) // ускорение не имеет ежеходного действия
{
    if (target == nullptr)
        throw std::invalid_argument("haste_exe -- target = nullptr!");
}

// замедление, -2 к мод. броска движения
void slowdown_exe (Player* target, int) // замедление не имеет ежеходного действия
{
    if (target == nullptr)
        throw std::invalid_argument("slowdown_exe -- target = nullptr!");
}

// удача, +2 к броску во время ивента
void luck_exe (Player* target, int) // удача не имеет ежеходного действия
{
    if (target == nullptr)
        throw std::invalid_argument("luck_exe -- target = nullptr!");
}

// развеивание всех развеиваемых
void dispell_all_exe (Player* target, int)
{
    if (target == nullptr)
        throw std::invalid_argument("dispell_all_exe -- target = nullptr!");

    for (std::vector<Effect*>::iterator it = target->get_active_effects()->begin(); it != target->get_active_effects()->end(); it++)
    {
        if ((*it)->is_dispellable())
        {
            delete *it;
            it = target->get_active_effects()->erase(it);
        }
    }
}

// развеивание положительных развеиваемых
void dispell_positive_exe (Player* target, int)
{
    if (target == nullptr)
        throw std::invalid_argument("dispell_positive_exe -- target = nullptr!");

    for (std::vector<Effect*>::iterator it = target->get_active_effects()->begin(); it != target->get_active_effects()->end(); it++)
    {
        if ((*it)->get_effect_type() != "положительный" && (*it)->get_effect_type() != "отрицательный" && (*it)->get_effect_type() != "нейтральный")
            throw std::invalid_argument((*it)->get_effect_name() + " has invalid type");
        if ((*it)->is_dispellable() && (*it)->get_effect_type() == "положительный")
        {
            delete *it;
            it = target->get_active_effects()->erase(it);
        }
    }
}

// развеивание отрицательных развеиваемых
void dispell_negative_exe (Player* target, int)
{
    if (target == nullptr)
        throw std::invalid_argument("dispell_negative_exe -- target = nullptr!");

    for (std::vector<Effect*>::iterator it = target->get_active_effects()->begin(); it != target->get_active_effects()->end(); it++)
    {
        if ((*it)->get_effect_type() != "положительный" && (*it)->get_effect_type() != "отрицательный" && (*it)->get_effect_type() != "нейтральный")
            throw std::invalid_argument((*it)->get_effect_name() + " has invalid type");
        if ((*it)->is_dispellable() && (*it)->get_effect_type() == "отрицательный")
        {
            delete *it;
            it = target->get_active_effects()->erase(it);
        }
    }
}

std::map<std::string, V*> effects_exe =
    {
        {"экспериментальный эффект", &experiment_exe},
        {"регенерация", &regeneration_exe},
        {"горение", &burning_exe},
        {"отравление", &intoxication_exe},
        {"кровотечение", &bleeding_exe},
        {"обморожение", &frostbite_exe},
        {"шок", &shock_exe},
        {"слабость", &weakness_exe},
        {"усиление", &empower_exe},
        {"стойкость", &endurance_exe},
        {"ускорение", &haste_exe},
        {"замедление", &slowdown_exe},
        {"удача", &luck_exe},
        {"развеивание", &dispell_all_exe},
        {"развеивание отрицательных эффектов", &dispell_negative_exe},
        {"развеивание положительных эффектов", &dispell_positive_exe}
    };

Effect::Effect()
{
    effect_counter = 0;
    effect_duration = -1;
    effect_name = "безымянный";
    effect_type = "неизвестно";
    dispellable = 0;
    target = nullptr;
    execute_effect_ptr = nullptr;
}

Effect::Effect(std::string _effect_name, Player *_target)
{
    if (!eff_data->is_in_objects(_effect_name))
        throw std::invalid_argument(_effect_name + " is not contained in all_effects JSON object");

    if (!eff_data->get_object(_effect_name)->is_in_objects("special_chs"))
        throw std::invalid_argument(_effect_name + " JSONObject does not contain \"special_chs\" key-to-object");

    if (effects_exe.find(_effect_name) == effects_exe.end())
        throw std::invalid_argument(_effect_name + " is not contained in effects_exe func. ptr map");

    JSONObject* tmp = eff_data->get_object(_effect_name);
    effect_counter = std::stoi(tmp->get_value("effect_counter"));
    effect_duration = std::stoi(tmp->get_value("effect_duration"));
    effect_name = tmp->get_value("effect_name");
    effect_type = tmp->get_value("effect_type");
    dispellable = std::stoi(tmp->get_value("dispellable"));;
    target = _target;
    execute_effect_ptr = effects_exe.at(effect_name);

    JSONObject* chars = tmp->get_object("special_chs");
    for (const auto& i : *chars->get_name_to_value())
    {
        special_chs.emplace(i.first, std::stoi(i.second));
    }
}

Effect::Effect(std::string _effect_name, Player *_target, int dur)
{
    if (!eff_data->is_in_objects(_effect_name))
        throw std::invalid_argument(_effect_name + " is not contained in all_effects JSON object");

    if (!eff_data->get_object(_effect_name)->is_in_objects("special_chs"))
        throw std::invalid_argument(_effect_name + " JSONObject does not contain \"special_chs\" key-to-object");

    if (effects_exe.find(_effect_name) == effects_exe.end())
        throw std::invalid_argument(_effect_name + " is not contained in effects_exe func. ptr map");

    JSONObject* tmp = eff_data->get_object(_effect_name);
    effect_counter = std::stoi(tmp->get_value("effect_counter"));
    effect_duration = dur;
    effect_name = tmp->get_value("effect_name");
    effect_type = tmp->get_value("effect_type");
    dispellable = std::stoi(tmp->get_value("dispellable"));;
    target = _target;
    execute_effect_ptr = effects_exe.at(effect_name);

    JSONObject* chars = tmp->get_object("special_chs");
    for (const auto& i : *chars->get_name_to_value())
    {
        special_chs.emplace(i.first, std::stoi(i.second));
    }
}

Effect::Effect(std::string _effect_name, Player *_target, int dur, int counter)
{
    if (!eff_data->is_in_objects(_effect_name))
        throw std::invalid_argument(_effect_name + " is not contained in all_effects JSON object");

    if (!eff_data->get_object(_effect_name)->is_in_objects("special_chs"))
        throw std::invalid_argument(_effect_name + " JSONObject does not contain \"special_chs\" key-to-object");

    if (effects_exe.find(_effect_name) == effects_exe.end())
        throw std::invalid_argument(_effect_name + " is not contained in effects_exe func. ptr map");

    JSONObject* tmp = eff_data->get_object(_effect_name);
    effect_counter = counter;
    effect_duration = dur;
    effect_name = tmp->get_value("effect_name");
    effect_type = tmp->get_value("effect_type");
    dispellable = std::stoi(tmp->get_value("dispellable"));;
    target = _target;
    execute_effect_ptr = effects_exe.at(effect_name);

    JSONObject* chars = tmp->get_object("special_chs");
    for (const auto& i : *chars->get_name_to_value())
    {
        special_chs.emplace(i.first, std::stoi(i.second));
    }
}

std::string Effect::get_effect_name() const
{
	return effect_name;
}

std::string Effect::get_effect_type() const
{
	return effect_type;
}

int Effect::get_effect_duration() const
{
	return effect_duration;
}

int Effect::get_effect_counter() const
{
    return effect_counter;
}

std::map<std::string, int>& Effect::get_sp_chs()
{
    return special_chs;
}

bool Effect::is_dispellable() const
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

void Effect::inc_duration()
{
    effect_duration++;
}

void Effect::inc_counter()
{
	effect_counter++;
}

void Effect::dec_counter()
{
    effect_counter--;
}

void Effect::apply_effect()
{
    // проверка на наличие аналогичного эффекта у цели
    for (std::vector<Effect*>::iterator it = target->get_active_effects()->begin(); it != target->get_active_effects()->end(); it++)
    {
        if ((*it)->get_effect_name() == effect_name)
        {
            effect_counter = (*it)->get_effect_counter();

            // далее перед удалением необходимо передать уникальные для эффекта хар-ки
            for (const auto& i : (*it)->get_sp_chs())
                if (target->get_characteristics().find(i.first) == target->get_characteristics().end())
                    special_chs[i.first] = i.second;

            delete *it;
            target->get_active_effects()->erase(it);
            target->get_active_effects()->push_back(this);
            return;
        }
    }

    // изменение существующих хар-к цели
    for (const auto& i : special_chs)
    {
        if (target->get_characteristics().find(i.first) != target->get_characteristics().end())
            target->get_characteristics().at(i.first) += i.second;
    }

    target->get_active_effects()->push_back(this);
}

void Effect::execute_effect()
{
    if (execute_effect_ptr != nullptr)
        execute_effect_ptr(target, effect_counter);
}

void Effect::reverse_effect(Player& target)
{
    for (const auto& i : special_chs)
    {
        std::string ch = i.first;
        int val = i.second;
        if (target.get_characteristics().find(ch) != target.get_characteristics().end())
            target.get_characteristics().at(ch) -= val;
    }

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

    //размер контейнера special_chs
    size = special_chs.size();
    //запись рамзера item_characteristics
    out.write((char*)& size, sizeof(size));
    //цикл для записи всего контейнера
    for(const auto &i : special_chs)
    {
        //размер ключа
        size_t string_size = i.first.size() + 1;
        //запись размера ключ
        out.write((char*)& string_size, sizeof(string_size));
        //запись ключа
        out.write(i.first.c_str(), string_size);

        //запись значения
        out.write((char*)& i.second, sizeof(i.second));
    }

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

    //чтение размера контейнера special_chs
    in.read((char*)& size, sizeof(size));
    //очистка контейнера special_chs
    special_chs.clear();

    //цикл для заполнения контейнера данными
    for(int i = 0; i < size; i++)
    {
        //переменная для размера ключа
        size_t string_size;
        //чтение размера ключа
        in.read((char*)& string_size, sizeof(string_size));
        //присваивание ключу строки из пробелов длиной size-1, без /0
        std::string key(string_size - 1, ' ');
        //чтение ключа
        in.read(key.data(), string_size);

        //переменная для значения
        int value;
        //чтение занчения
        in.read((char*)& value, sizeof(value));

        //добавление или изменение пары ключ-значение
        special_chs[key] = value;
    }

    //чтение dispallable
    in.read((char*)& dispellable, sizeof(dispellable));
}
