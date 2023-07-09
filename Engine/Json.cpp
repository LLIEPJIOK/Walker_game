
#include "Json.h"

JSONObject::JSONObject()
{
    name_to_values = new std::unordered_map <std::string, std::vector<std::string>* >;
    name_to_object = new std::unordered_map <std::string, JSONObject*>;
    name_to_objects = new std::unordered_map <std::string, std::vector<JSONObject*>* >;
    name_to_value = new std::unordered_map <std::string, std::string>;
}

JSONObject::~JSONObject()
{
    for (auto& i : *name_to_object)
        delete i.second;
    delete name_to_object;

    for(auto& i : *name_to_objects)
    {
        for(auto& j : *i.second)
            delete j;
        delete i.second;
    }
    delete name_to_objects;

    for(auto& i : *name_to_values)
        delete i.second;
    delete name_to_values;

    delete name_to_value;
}

JSONObject::JSONObject(std::string text)
{
    name_to_values = new std::unordered_map <std::string, std::vector<std::string>* >;
    name_to_object = new std::unordered_map <std::string, JSONObject*>;
    name_to_objects = new std::unordered_map <std::string, std::vector<JSONObject*>* >;
    name_to_value = new std::unordered_map <std::string, std::string>;

    int block_counter = (int)std::min(count(text.begin(), text.end(), '{'), count(text.begin(), text.end(), '}'));


    int pos1 = text.find_first_of("{"), pos2 = 0;
    while (block_counter > 0)
    {
        pos1 = text.find_first_of("\"", pos1);
        if (pos1 == -1)
            break;
        pos1++;
        pos2 = text.find_first_of("\"", pos1);
        if (pos2 == -1)
            break;
        std::string key = text.substr(pos1, pos2 - pos1);
        std::string value = "";
        pos1 = pos2 + 1;
        int check_pos = text.find_first_of("{[", pos1);

        if (check_pos > text.find_first_of("\"", pos1)) // if it is a default name to value data
        {
            pos1 = text.find_first_of("\"", pos1);
            if (pos1 == -1)
                break;
            pos1++;
            pos2 = text.find_first_of("\"", pos1);
            if (pos2 == -1)
                break;
            value = text.substr(pos1, pos2 - pos1);
            pos1 = pos2 + 1;
            name_to_value->emplace(std::make_pair(key, value));
        }

        else if (text[check_pos] == '{') // if it is a beginning of a JSONObject
        {
            int end_pos = find_block_end_1(text, check_pos);
            JSONObject* tmp = new JSONObject(text.substr(check_pos, end_pos - check_pos + 1));
            name_to_object->emplace(std::make_pair(key, tmp));
            pos1 = end_pos + 1;
        }
        else if (text[check_pos] == '[') // if it is a beginning of an array
        {
            int end_pos = find_block_end_2(text, check_pos);
            std::string array = text.substr(check_pos, end_pos - check_pos + 1);
            if(array.find_first_of("{}", 0) == -1)
                insert_array(key, array);
            else
                insert_object_array(key, array);
            pos1 = end_pos + 1;
        }
    }
}

JSONObject::JSONObject(const JSONObject& other)
{
    name_to_values = new std::unordered_map <std::string, std::vector<std::string>* >;
    for (const auto& i : *other.name_to_values)
    {
        std::vector<std::string>* tmp = new std::vector<std::string>;
        for (const auto& j : *i.second)
            tmp->push_back(j);
    }

    name_to_object = new std::unordered_map <std::string, JSONObject*>;
    for (const auto& i : *other.name_to_object)
    {
        JSONObject* tmp = new JSONObject(*i.second);
        name_to_object->emplace(std::make_pair(i.first, tmp));
    }

    name_to_objects = new std::unordered_map <std::string, std::vector<JSONObject*>* >;
    for (const auto& i : *other.name_to_objects)
    {
        std::vector<JSONObject*>* tmp1 = new std::vector<JSONObject*>;
        for (const auto& j : *i.second)
        {
            JSONObject* tmp2 = new JSONObject(*j);
            tmp1->push_back(tmp2);
        }
        name_to_objects->emplace(std::make_pair(i.first, tmp1));
    }

    name_to_value = new std::unordered_map <std::string, std::string>;
    for (const auto& i : *other.name_to_value)
        name_to_value->emplace(std::make_pair(i.first, i.second));
}

JSONObject* JSONObject::get_object(std::string key)
{
    return name_to_object->at(key);
}

std::vector<JSONObject*>* JSONObject::get_objects(std::string key)
{
    return name_to_objects->at(key);
}

std::vector<std::string>* JSONObject::get_values(std::string key)
{
    return name_to_values->at(key);
}

std::string JSONObject::get_value(std::string key)
{
    return name_to_value->at(key);
}

std::unordered_map<std::string, JSONObject*>* JSONObject::get_name_to_object()
{
    return name_to_object;
}

std::unordered_map<std::string, std::vector<JSONObject*>*>* JSONObject::get_name_to_objects()
{
    return name_to_objects;
}

std::unordered_map<std::string, std::vector<std::string>*>* JSONObject::get_name_to_values()
{
    return name_to_values;
}

std::unordered_map<std::string, std::string>* JSONObject::get_name_to_value()
{
    return name_to_value;
}

bool JSONObject::is_in_values(std::string key)
{
    if (name_to_value->find(key) == name_to_value->end())
        return false;
    return true;
}

bool JSONObject::is_in_objects(std::string key)
{
    if (name_to_object->find(key) == name_to_object->end())
        return false;
    return true;
}

bool JSONObject::is_in_arrays(std::string key)
{
    if (name_to_values->find(key) == name_to_values->end())
        return false;
    return true;
}

bool JSONObject::is_in_object_arrays(std::string key)
{
    if (name_to_objects->find(key) == name_to_objects->end())
        return false;
    return true;
}

int JSONObject::find_block_end_1(std::string& text, int curpos)
{
    int pos1 = curpos;
    int block_c = 1;
    while (block_c > 0)
    {
        pos1++;
        pos1 = text.find_first_of("{}", pos1);
        if (text[pos1] == '{')
            block_c++;
        else
            block_c--;
    }
    return pos1;
}

int JSONObject::find_block_end_2(std::string& text, int curpos)
{
    int pos1 = curpos;
    int block_c = 1;
    while (block_c > 0)
    {
        pos1++;
        pos1 = text.find_first_of("[]", pos1);
        if (text[pos1] == '[')
            block_c++;
        else
            block_c--;
    }
    return pos1;
}

void JSONObject::operator=(const JSONObject& other)
{
    name_to_values = new std::unordered_map <std::string, std::vector<std::string>* >;
    for (const auto& i : *other.name_to_values)
    {
        std::vector<std::string>* tmp = new std::vector<std::string>;
        for (const auto& j : *i.second)
            tmp->push_back(j);
    }

    name_to_object = new std::unordered_map <std::string, JSONObject*>;
    for (const auto& i : *other.name_to_object)
    {
        JSONObject* tmp = new JSONObject(*i.second);
        name_to_object->emplace(std::make_pair(i.first, tmp));
    }

    name_to_objects = new std::unordered_map <std::string, std::vector<JSONObject*>* >;
    for (const auto& i : *other.name_to_objects)
    {
        std::vector<JSONObject*>* tmp1 = new std::vector<JSONObject*>;
        for (const auto& j : *i.second)
        {
            JSONObject* tmp2 = new JSONObject(*j);
            tmp1->push_back(tmp2);
        }
        name_to_objects->emplace(std::make_pair(i.first, tmp1));
    }

    name_to_value = new std::unordered_map <std::string, std::string>;
    for (const auto& i : *other.name_to_value)
        name_to_value->emplace(std::make_pair(i.first, i.second));
}

void JSONObject::insert_object_array(std::string key, std::string array)
{
    int pos1 = array.find_first_of("{",0), pos2;
    std::vector<JSONObject*>* objects = new std::vector<JSONObject*>;
    while(pos1 != -1)
    {
        pos2 = find_block_end_1(array, pos1);
        std::string text = array.substr(pos1, pos2 - pos1 + 1);
        JSONObject* object = new JSONObject(text);
        objects->push_back(object);
        pos1 = array.find_first_of("{", pos2 + 1);
    }
    name_to_objects->emplace(std::make_pair(key, objects));

}

void JSONObject::insert_array(std::string key, std::string array)
{
    int pos2, pos1 = 0;
    pos1 = array.find_first_of("\"", pos1);
    std::vector<std::string>* values = new std::vector<std::string>;
    while(pos1 != -1)
    {
        pos2 = array.find_first_of("\"",pos1 + 1);
        std::string value = array.substr(pos1 + 1, pos2 - pos1);
        values->push_back(value);
        pos1 = array.find_first_of("\"", pos2 + 1);
    }
    name_to_values->emplace(std::make_pair(key, values));
}
