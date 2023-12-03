
#include "Event.h"
#include "Engine/Effect.h"
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
        return "../Game/Resources/Pictures/missingContentError.png";

    return path;
}

Event::Event(JSONObject* tmp)
{
    std::string cur = "event_name";
    if (!tmp->is_in_values(cur)) // каждый раз проверяет, есть ли значение, соответствующее данному ключу.
        throw std::invalid_argument(cur + " is not contained in JSON of Events");

    event_name = tmp->get_value("event_name");

    cur = "img_path";
    if (!tmp->is_in_values(cur))
        throw std::invalid_argument(cur + " is not contained in JSON of" + event_name);

    img_path = from_invalid_to_missing_png("../Game/Resources/Pictures/" + tmp->get_value("img_path"));

    cur = "event_info";
    if (!tmp->is_in_values(cur))
        throw std::invalid_argument(cur + " is not contained in JSON of" + event_name);

    event_info = tmp->get_value(cur);

    cur = "option";
    if (!tmp->is_in_values(cur))
        throw std::invalid_argument(cur + " is not contained in JSON of" + event_name);

    option = tmp->get_value("option");

    cur = "success_text";
    if (!tmp->is_in_values(cur))
        throw std::invalid_argument(cur + " is not contained in JSON of" + event_name);

    success_text = tmp->get_value("success_text");

    cur = "failure_text";
    if (!tmp->is_in_values(cur))
        throw std::invalid_argument(cur + " is not contained in JSON of" + event_name);

    failure_text = tmp->get_value("failure_text");

    cur = "success_image";
    if (!tmp->is_in_values(cur))
        throw std::invalid_argument(cur + " is not contained in JSON of" + event_name);

    success_image = from_invalid_to_missing_png("../Game/Resources/Pictures/" + tmp->get_value("success_image"));

    cur = "failure_image";
    if (!tmp->is_in_values(cur))
        throw std::invalid_argument(cur + " is not contained in JSON of" + event_name);

    failure_image = from_invalid_to_missing_png("../Game/Resources/Pictures/" + tmp->get_value("failure_image"));

    cur = "type";
    if (!tmp->is_in_values(cur))
        throw std::invalid_argument(cur + " is not contained in JSON of" + event_name);

    type = tmp->get_value("type");

    cur = "requirement";
    if (!tmp->is_in_values(cur))
        throw std::invalid_argument(cur + " is not contained in JSON of" + event_name);

    requirement = stoi(tmp->get_value("requirement"));

}

std::string Event::get_event_name()
{
    return event_name;
}

std::string Event::get_img_path()
{
    return img_path;
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

std::string Event::get_success_image()
{
    return success_image;
}

std::string Event::get_failure_image()
{
    return failure_image;
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
    case 0: pl->add_item("Загадочное кольцо");
        break;
    case 1: pl->add_item("Загадочное ожерелье");
        break;
    case 2: pl->add_item("Загадочный пояс");
        break;
    }
}

void empty_house_event::execute_failure(Player *pl)
{
    Effect* eff = new Effect("слабость", pl);
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
    case 0: pl->add_item("Зелье регенерации");
        break;
    case 1: pl->add_item("Зелье силы");
        break;
    case 2: pl->add_item("Зелье стойкости");
        break;
    case 3: pl->add_item("Зелье ускорения");
        break;
    case 4:
{
        int stat = pl->get_characteristics().at("INT");
        if (stat < 5)
        {
            pl->add_item("Малое зелье лечения");
        }
        else if (stat < 10)
        {
            pl->add_item("Зелье лечения");
        }
        else
        {
            pl->add_item("Большое зелье лечения");
        }
        break;
}
    }
}

void mushrooms_event::execute_failure(Player *pl)
{
    Effect* eff = new Effect("отравление", pl);
    eff->apply_effect();
}

Events::Events()
{
    std::ifstream fin("../Game/Resources/Files/Events.txt");
    std::string info = "", line;
    while(fin.good())
    {
        std::getline(fin, line);
        info+= line;
    }

    JSONObject events_info(info);

    // убрать ивент из пула ивентов безболезненно можно здесь, закомментив нужный
    // events.emplace(std::make_pair("experiment", new experiment_event(events_info.get_object("experiment")))); // ничего не делает
    events.emplace(std::make_pair("loggers", new loggers_event(&events_info.get_object("loggers"))));
    events.emplace(std::make_pair("empty house", new empty_house_event(&events_info.get_object("empty house"))));
    events.emplace(std::make_pair("mushrooms", new mushrooms_event(&events_info.get_object("mushrooms"))));

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
