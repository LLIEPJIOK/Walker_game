
#ifndef EVENT_H
#define EVENT_H

#include "Json/json.hpp"
#include "Engine/Player.h"

using json = nlohmann::json;

class Event
{

protected:
    std::string event_name;
    std::string event_info;
    std::string success_text;
    std::string failure_text;
    std::string option;
    std::string type;
    int requirement;

public:
    Event();
    Event(json&);

    std::string get_type();
    std::string get_event_name();
    std::string get_event_info();
    std::string get_option();
    std::string get_success_text();
    std::string get_failure_text();

    virtual int get_requirement(Player* pl); // в зависимости от статов игрока возвращает необходимое значение (ролл + стат) для успеха
    virtual void execute_success(Player* pl) = 0; // выполняет задуманную разработчиком функцию, связанную с успехом события
    virtual void execute_failure(Player* pl) = 0; // выполняет задуманную разработчиком функцию, связанную с провалом события
};

class experiment_event : public Event
{
public:
    experiment_event(json& tmp) : Event(tmp){}
    void execute_success(Player*);
    void execute_failure(Player*);
};

class loggers_event : public Event
{
public:
    loggers_event(json& tmp) : Event(tmp){}
    int get_requirement(Player* pl);
    void execute_success(Player* pl);
    void execute_failure(Player* pl);
};

class empty_house_event : public Event
{
public:
    empty_house_event(json& tmp) : Event(tmp){}
    int get_requirement(Player* pl);
    void execute_success(Player* pl);
    void execute_failure(Player* pl);
};

class mushrooms_event : public Event
{
public:
    mushrooms_event(json& tmp) : Event(tmp){}
    int get_requirement(Player* pl);
    void execute_success(Player* pl);
    void execute_failure(Player* pl);
};

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
