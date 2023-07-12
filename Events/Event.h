
#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <vector>
#include <QObject>
#include <map>
#include <fstream>
#include "Engine/Json.h"
#include "Engine/Player.h"

class Event
{

protected:
    std::string event_name;
    std::string img_path;
    std::string event_info;
    std::string success_text;
    std::string failure_text;
    std::string success_image;
    std::string failure_image;
    std::string option;
    std::string type;
    int requirement;

public:
    Event();
    Event(JSONObject*);

    std::string get_type();
    std::string get_event_name();
    std::string get_img_path();
    std::string get_event_info();
    std::string get_option();
    std::string get_success_text();
    std::string get_failure_text();
    std::string get_success_image();
    std::string get_failure_image();

    int get_requirement();

    virtual void execute_success() = 0;
    virtual void execute_failure() = 0;
};

class experiment_event : public Event
{
public:
    experiment_event(JSONObject* tmp) : Event(tmp){}
    void execute_success();
    void execute_failure();
};


/*class Small_healing_event : public Event
{
public:
    Small_healing_event();
    void execute(Player&);
};

class Slowdown_event : public Event
{
public:
    Slowdown_event();
    void execute(Player&);
};

class Haste_event : public Event
{
public:
    Haste_event();
    void execute(Player&);
};

class Intoxication_event : public Event
{
    Intoxication_event();
    void execute(Player&);
};

class Die_insect_event : public Event
{
    Die_insect_event();
    void execute(Player&);
}; */

class Events
{
private:
    std::map<std::string, Event*> events;
    static Events* Events_data;
public:
    Events();
    Events(const Events&) = delete;
    const Events& operator = (const Events&) = delete;
    std::map<std::string, Event* >* get_events();
    static Events* get_Events();
};



#endif // EVENT_H