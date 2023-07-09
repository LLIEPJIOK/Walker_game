
#include "Event.h"
#include "event_window.h"

Events* Events::Events_data = 0;

//void Event::dispell(Player& target)
//{
//
//	for (auto i = target.get_active_effects()->begin(); i != target.get_active_effects()->end(); i++)
//	{
//		if((*i)->is_dispellable())
//		{
//			delete* i;
//			i = target.get_active_effects()->erase(i);
//		}
//	}
//}


//void Events::charming_mermaid(Player& target)
//{
//	std::map<std::string, int> mermaid_charms;
//	mermaid_charms.emplace(std::make_pair("ARM", 256));
//	mermaid_charms.emplace(std::make_pair("QNT", -1));
//	target.get_active_effects()->push_back(new Effect("charmed", "Temporary", "charming_mermaid", &mermaid_charms, 2, 1));
//
//	std::map<std::string, int> mermaid_blessing;
//	mermaid_blessing.emplace(std::make_pair("HP", 2));
//	mermaid_blessing.emplace(std::make_pair("MAXHP", 1));
//	target.get_active_effects()->push_back(new Effect("charmed", "TemporaryConst", "charming_mermaid", &mermaid_blessing, 5, 1));
//}

//
//
//
//Events* Events::Events_data = 0;
//
//Event::Event()
//{
//	event_name = "";
//}
//
//Event::Event(std::string _event_name)
//{
//	Event::event_name = _event_name;
//}
//
//std::string Event::get_name()
//{
//	return event_name;
//}
//
//Small_healing_event::Small_healing_event()
//{
//	event_name = "Small_healing_event";
//}
//
//void Small_healing_event::execute(Player& target)
//{
//	for (const auto& i : *target.get_active_effects())
//		if (i->get_effect_name() == "Small_healing")
//		{
//			i->set_effect_duration(6);
//			return;
//		}
//	std::map<std::string, int> healing_chars;
//	healing_chars.emplace(std::make_pair("HP", 1));
//	target.get_active_effects()->push_back(new Effect("Small_healing", "TemporaryConst", &healing_chars, 6, 1));
//}
//
//Slowdown_event::Slowdown_event()
//{
//	event_name = "Slowdown_event";
//}
//
//void Slowdown_event::execute(Player& target)
//{
//
//	for (const auto& i : *target.get_active_effects())
//		if (i->get_effect_name() == "Slowddown")
//		{
//			i->set_effect_duration(2);
//			return;
//		}
//	std::map<std::string, int> slowdown_chars;
//	slowdown_chars.emplace(std::make_pair("ROLL", -1));
//	target.get_active_effects()->push_back(new Effect("Slowdown", "Temporary", &slowdown_chars, 2, 1));
//}
//
//Haste_event::Haste_event()
//{
//	event_name = "Haste_event";
//}
//
//void Haste_event::execute(Player& target)
//{
//	for (const auto& i : *target.get_active_effects())
//		if (i->get_effect_name() == "Haste")
//		{
//			i->set_effect_duration(2);
//			return;
//		}
//	std::map<std::string, int> haste_chars;
//	haste_chars.emplace(std::make_pair("QNT", 1));
//	target.get_active_effects()->push_back(new Effect("Haste", "Temporary", &haste_chars, 2, 1));
//}
//
//Events::~Events()
//{
//	for (const auto& i : *events)
//		delete i;
//
//}
//
//Events::Events()
//{
//	events = new std::vector<Event*>;
//
//	events->push_back(new Small_healing_event());
//	events->push_back(new Slowdown_event());
//	events->push_back(new Haste_event());
//
//}
//
//Intoxication_event::Intoxication_event()
//{
//	event_name = "Intoxication_event";
//}
//
//void Intoxication_event::execute(Player& target)
//{
//	for (const auto& i : *target.get_active_effects())
//			if (i->get_effect_name() == "Intoxication")
//			{
//				i->set_effect_duration(3);
//				return;
//			}
//		std::map<std::string, int> poison_chars;
//		poison_chars.emplace(std::make_pair("HP", -1));
//		target.get_active_effects()->push_back(new Effect("Intoxication", "TemporaryConst", &poison_chars, 3, 1));
//}
//
//Die_insect_event::Die_insect_event()
//{
//	event_name = "Die_insect_event";
//}
//
//void Die_insect_event::execute(Player& target)
//{
//	int subj = rand() % DataBase::get_DataBase("in.txt")->get_sequence()->size() + 1;
//	std::map<std::string, Player*>::iterator it = DataBase::get_DataBase("in.txt")->get_sequence()->begin();
//	for (int i = 0; i < subj; it++)
//		;
//	it->second->get_characteristics()["HP"] -= 8;
//}
//
//std::vector<Event*>* Events::get_events()
//{
//	return events;
//}
//
//Events* Events::get_Events()
//{
//	if (!Events_data)
//		Events_data = new Events();
//	return Events_data;
//}


Event::Event(JSONObject* tmp)
{
    event_name = tmp->get_value("event_name");
    img_path = "../Game/Resources/Pictures/" + tmp->get_value("img_path");
    event_info = tmp->get_value("event_info");
    option = tmp->get_value("option");
    success_text = tmp->get_value("success_text");
    failure_text = tmp->get_value("failure_text");
    success_image = "../Game/Resources/Pictures/" + tmp->get_value("success_image");
    failure_image = "../Game/Resources/Pictures/" + tmp->get_value("failure_image");
    type = tmp->get_value("type");
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

int Event::get_requirement()
{
    return requirement;
}


void experiment_event::execute_success()
{

}

void experiment_event::execute_failure()
{

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
    events.emplace(std::make_pair("experiment", new experiment_event(events_info.get_object("experiment"))));

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

