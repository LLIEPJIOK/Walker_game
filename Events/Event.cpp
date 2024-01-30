
#include "Event.h"
#include "Engine/Effect.h"
#include <QTextStream>
#include <sys/stat.h> // для проверки валидности пути файла

Events* Events::Events_data = 0;

std::string from_invalid_to_missing_png(std::string path) // если путь картинки неверный, то картинка будет заменена на missingContentError.png
{
    // Path to the directory
    const char* file = path.c_str();

    // Structure which would store the metadata
    struct stat sb;

    // Calls the function with path as argument
    // If the file/directory exists at the path returns 0
    // If block executes if path exists
    if (stat(file, &sb) != 0 || (sb.st_mode & S_IFDIR))
        return ":/events/Pictures/Events/missingContentError.png";

    return path;
}

Event::Event(json& tmp)
{
    event_name = tmp["event_name"];

    event_info = tmp["event_info"];

    option = tmp["option"];

    success_text = tmp["success_text"];

    failure_text = tmp["failure_text"];

    type = tmp["type"];

    requirement = tmp["requirement"];

}

std::string Event::get_event_name()
{
    return event_name;
}

std::string Event::get_event_info()
{
    return event_info;
}

std::string Event::get_option()
{
    return option;
}

std::string Event::get_success_text()
{
    return success_text;
}


std::string Event::get_failure_text()
{
    return failure_text;
}

std::string Event::get_type()
{
    return type;
}

int Event::get_requirement(Player*)
{
    return requirement;
}


void experiment_event::execute_success(Player*)
{

}

void experiment_event::execute_failure(Player*)
{

}

int loggers_event::get_requirement(Player *pl)
{
    int luck_reduction = pl->get_characteristics().at("EVENT_ROLL_MOD") / 2;
    int stat = pl->get_characteristics().at("STR");
    if (stat < 12)
    {
        return stat + 3 + luck_reduction;
    }
    else
    {
        return stat + 5 + luck_reduction;
    }
}

void loggers_event::execute_success(Player* pl)
{
    int healed = (pl->get_characteristics().at("MAX_HP") - pl->get_characteristics().at("HP")) * 0.1; // 10% от разницы
    pl->get_characteristics().at("HP") += healed;
    pl->get_characteristics().at("STR") += 1;
}

void loggers_event::execute_failure(Player* pl)
{
    int damaged = pl->get_characteristics().at("HP") * 0.1; // 10% от текущего хп
    pl->get_characteristics().at("HP") -= damaged;
}

int empty_house_event::get_requirement(Player *pl)
{
    int luck_reduction = pl->get_characteristics().at("EVENT_ROLL_MOD") / 2;
    int stat = pl->get_characteristics().at("AGIL");
    if (stat < 10)
    {
        return stat + 4 + luck_reduction;
    }
    else if (stat < 16)
    {
        return stat + 3 + luck_reduction;
    }
    else
    {
        return stat + 2 + luck_reduction;
    }
}

void empty_house_event::execute_success(Player *pl)
{
    int type = rand() % 3;
    switch (type)
    {
    case 0: pl->add_item("Mystery ring");
        break;
    case 1: pl->add_item("Mystery necklace");
        break;
    case 2: pl->add_item("Mystery belt");
        break;
    }
}

void empty_house_event::execute_failure(Player *pl)
{
    Effect* eff = new Effect("Weakness", pl);
    eff->apply_effect();
}

int mushrooms_event::get_requirement(Player *pl)
{
    int luck_reduction = pl->get_characteristics().at("EVENT_ROLL_MOD") / 2;
    int stat = pl->get_characteristics().at("INT");
    if (stat < 8)
    {
        return stat + 3 + luck_reduction;
    }
    else
    {
        return stat + 2 + luck_reduction;
    }
}

void mushrooms_event::execute_success(Player *pl)
{
    int type = rand() % 5;
    switch (type)
    {
    case 0: pl->add_item("Potion of regeneration");
        break;
    case 1: pl->add_item("Potion of power");
        break;
    case 2: pl->add_item("Potion of endurance");
        break;
    case 3: pl->add_item("Potion of haste");
        break;
    case 4:
{
        int stat = pl->get_characteristics().at("INT");
        if (stat < 5)
        {
            pl->add_item("Lesser healing potion");
        }
        else if (stat < 10)
        {
            pl->add_item("Healing potion");
        }
        else
        {
            pl->add_item("Great healing potion");
        }
        break;
}
    }
}

void mushrooms_event::execute_failure(Player *pl)
{
    Effect* eff = new Effect("Intoxication", pl);
    eff->apply_effect();
}

Events::Events()
{
    std::ifstream fin("../Game/Resources/Files/Events.txt");

    json events_info = json::parse(fin);
    fin.close();

    // убрать ивент из пула ивентов безболезненно можно здесь, закомментив нужный
    // events.emplace(std::make_pair("experiment", new experiment_event(events_info.get_object("experiment")))); // ничего не делает
    events.emplace(std::make_pair("loggers", new loggers_event(events_info["loggers"])));
    events.emplace(std::make_pair("empty_house", new empty_house_event(events_info["empty_house"])));
    events.emplace(std::make_pair("mushrooms", new mushrooms_event(events_info["mushrooms"])));

}

std::map<std::string, Event *> *Events::get_events()
{
    return &events;
}

Events *Events::get_Events()
{
    if(!Events_data)
        Events_data = new Events;
    return Events_data;
}
