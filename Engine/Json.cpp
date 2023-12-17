
#include "Json.h"
#include<stack>
#include<stdexcept>
#include<fstream>

enum expecting{key, value, object, array, object_array, unidentified, unidentified_array};

JSONObject::JSONObject(std::string& source) // ???
{
    size_t begin = source.find_first_of('{', 0);
    if (begin == -1)
        throw new std::invalid_argument("source");

    std::string k;
    expecting current = key;
    while (has_next_key(source, ++begin)) {

        // pick the key
        k = pick_val(source, begin);
        begin++;

        // identify target field using control symbols \", { or [
        for (; begin < source.size(); begin++) {
            if (source[begin] == '\"')
                current = value;
            else if (source[begin] == '{')
                current = object;
            else if (source[begin] == '[')
                current = unidentified_array;
            else
                continue;

            if (begin == source.size())
                throw new std::invalid_argument("Key does not correspond to anything! Key = " + k);

            break;
        }

        // if the target is an array, determine the type of the array using control symbols \", {
        if (current == unidentified_array) {
            size_t pos_obj = source.find_first_of('{', begin);
            size_t pos_val = source.find_first_of('\"', begin);
            size_t pos_ctrl = source.find_first_of(']', begin);
            if (pos_ctrl == -1)
                throw new std::invalid_argument("Key " + k + "corresponds to invalid array");

            if (pos_ctrl < pos_val && pos_ctrl < pos_obj)
                current = array;
            else {
                bool v = pos_val != -1;
                bool o = pos_obj != -1;
                if (!v && !o)
                    return;

                if (pos_obj < pos_val) {
                    if (o) {
                        current = object_array;
                    }
                    else if (v) {
                        current = array;
                    }
                }
                else if (v)
                    current = array;
                else
                    current = object_array;
            }
        }

        // after identifying the target, pick it and insert/rewrite
        if (current == value) {
            this->key_to_value[k] = pick_val(source, begin);
        }
        else if (current == object) {
            JSONObject tmp(source, begin);
            this->key_to_object[k] = tmp;
        }
        else if (current == array) {
            std::list<std::string> tmp = pick_val_list(source, begin);
            this->key_to_value_list[k] = tmp;
        }
        else if (current == object_array) {
            std::list<JSONObject> tmp = pick_obj_list(source, begin);
            this->key_to_object_list[k] = tmp;
        }
    }
}

JSONObject::JSONObject(std::string& source, size_t &begin) // ???
{
    begin = source.find_first_of('{', begin);
    if (begin == -1)
        throw new std::invalid_argument("source");

    std::string k;
    expecting current = key;
    while (has_next_key(source, ++begin)) {

        // pick the key
        k = pick_val(source, begin);
        begin++;

        // identify target field using control symbols \", { or [
        for (; begin < source.size(); begin++) {
            if (source[begin] == '\"')
                current = value;
            else if (source[begin] == '{')
                current = object;
            else if (source[begin] == '[')
                current = unidentified_array;
            else
                continue;

            if (begin == source.size())
                return;

            break;
        }

        // if the target is an array, determine the type of the array using control symbols \", {
        if (current == unidentified_array) {
            size_t pos_obj = source.find_first_of('{', begin);
            size_t pos_val = source.find_first_of('\"', begin);
            size_t pos_ctrl = source.find_first_of(']', begin);
            if (pos_ctrl == -1)
                throw new std::invalid_argument("Key " + k + " corresponds to invalid array");

            if (pos_ctrl < pos_val && pos_ctrl < pos_obj)
                current = array;
            else {
                bool v = pos_val != -1;
                bool o = pos_obj != -1;
                if (!v && !o)
                    return;

                if (pos_obj < pos_val) {
                    if (o) {
                        current = object_array;
                    }
                    else if (v) {
                        current = array;
                    }
                }
                else if (v)
                    current = array;
                else
                    current = object_array;
            }
        }

        // after identifying the target, pick it and insert/rewrite
        if (current == value) {
            this->key_to_value[k] = pick_val(source, begin);
        }
        else if (current == object) {
            JSONObject tmp(source, begin);
            this->key_to_object[k] = tmp;
        }
        else if (current == array) {
            std::list<std::string> tmp = pick_val_list(source, begin);
            this->key_to_value_list[k] = tmp;
        }
        else if (current == object_array) {
            std::list<JSONObject> tmp = pick_obj_list(source, begin);
            this->key_to_object_list[k] = tmp;
        }
    }
}

int JSONObject::find_block_end_array(const std::string& source, int curpos)
{
    int pos = curpos + 1;
    bool counts = true;
    while (pos < source.size()) {
        if (source[pos] == '\"') {
            if (source[pos - 1] != '\\')
                counts = (counts + 1) % 2;
        }
        else if (counts && source[pos] == ']')
            return pos;

        pos++;
    }

    return -1;
}

std::string JSONObject::pick_val(std::string& source, size_t &begin) // (OK)
{
    if (begin + 1 >= source.size())
        throw new std::invalid_argument("begin");

    begin = source.find_first_of('\"', begin);
    if (begin + 1 > source.size() || begin == -1)
        throw new std::invalid_argument("source");

    begin++;
    std::stack<char> pile;
    while (begin < source.size()) {
        char cur = source[begin];
        if (cur == '\\') {
            if (begin + 1 < source.size()) {
                char next = source[begin + 1];
                if (next == '\\')
                    pile.push(cur);
                else if (next == '"')
                    pile.push('\"');
                else if (next == 'b')
                    pile.pop();
                else if (next == 't')
                    pile.push('\t');

                begin += 2;
            }
            else {
                throw new std::invalid_argument("source");
            }
        }
        else if (cur == '\"')
            break;
        else {
            pile.push(cur);
            begin++;
        }
    }

    if (begin >= source.size())
        throw new std::invalid_argument("source");

    std::string res(pile.size(), '0');
    while (pile.size() > 0) {
        res[pile.size() - 1] = pile.top();
        pile.pop();
    }

    return res;
}

std::list<std::string> JSONObject::pick_val_list(std::string& source, size_t &begin)
{
    begin = source.find_first_of('[', begin);
    if (begin == -1)
        throw new std::invalid_argument("source");

    size_t end = find_block_end_array(source, begin);
    if (end == -1)
        throw new std::invalid_argument("source");

    std::list<std::string> values;
    while (begin < end && has_next_key(source, begin))
    {
        std::string value = pick_val(source, begin);
        values.push_back(value);
        begin++;
    }

    return values;
}

std::list<JSONObject> JSONObject::pick_obj_list(std::string& source, size_t &begin)
{
    size_t pos1 = source.find_first_of('{', begin);
    size_t pos2 = source.find_first_of(']', begin);

    if (pos2 == -1)
        throw new std::invalid_argument("invalid source array. Position = " + std::to_string(begin));

    std::list<JSONObject> tmp;
    if (pos1 == -1 || pos2 < pos1) {
        begin = pos2;
        return tmp;
    }

    //std::string source = "{\"TestObjects\"[{    }     {}{\"key\"\"val\"}{}    {}{\"objs\"[{}{}]}]}";

    while (pos1 != -1) {

        JSONObject obj(source, pos1);
        begin = source.find_first_of(']', pos1);
        if (begin == -1)
            throw new std::invalid_argument("invalid source array. Position = " + std::to_string(begin));

        tmp.push_back(obj);
        pos1++;

        pos1 = source.find_first_of('{', pos1);

        if (pos1 > begin) {
            begin = pos2;
            break;
        }
    }
    return tmp;
}

bool JSONObject::has_next_key(std::string& source, size_t &pos)
{
    size_t end = source.find_first_of('}', pos);
    if (end == -1)
        throw new std::invalid_argument("Source file does not contain closing bracket for current object. Latest position = " + std::to_string(pos));

    size_t begin = source.find_first_of('\"', pos);
    if (begin == -1 || begin > end) {
        pos = end;
        return false;
    }

    begin++;
    while (begin < source.size()) {
        if (source[begin] == '\"' && source[begin - 1] != '\\') {
            if (source.find_first_of('}', begin) == -1)
                throw new std::invalid_argument("Source file does not contain closing bracket for current object. Latest position = " + std::to_string(pos));

            return true;
        }

        begin++;
    }

    return false;
}

JSONObject& JSONObject::get_object(std::string key)
{
    return key_to_object.at(key);
}

std::list<JSONObject>& JSONObject::get_obj_list(std::string key)
{
    return key_to_object_list.at(key);
}

std::list<std::string>& JSONObject::get_val_list(std::string key)
{
    return key_to_value_list.at(key);
}

std::string& JSONObject::get_value(std::string key)
{
    return key_to_value.at(key);
}

const std::unordered_map<std::string, JSONObject>& JSONObject::get_name_to_object() const
{
    return key_to_object;
}

const std::unordered_map<std::string, std::list<JSONObject> >& JSONObject::get_name_to_objects() const
{
    return key_to_object_list;
}

const std::unordered_map<std::string, std::list<std::string> >& JSONObject::get_name_to_values() const
{
    return key_to_value_list;
}

const std::unordered_map<std::string, std::string>& JSONObject::get_name_to_value() const
{
    return key_to_value;
}

bool JSONObject::is_in_values(std::string key) const
{
    if (key_to_value.find(key) == key_to_value.end())
        return false;

    return true;
}

bool JSONObject::is_in_objects(std::string key) const
{
    if (key_to_object.find(key) == key_to_object.end())
        return false;

    return true;
}

bool JSONObject::is_in_val_lists(std::string key) const
{
    if (key_to_value_list.find(key) == key_to_value_list.end())
        return false;

    return true;
}

bool JSONObject::is_in_obj_lists(std::string key) const
{
    if (key_to_object_list.find(key) == key_to_object_list.end())
        return false;

    return true;
}

bool JSONObject::empty_values() const
{
    return key_to_value.size() == 0;
}

bool JSONObject::empty_objects() const
{
    return key_to_object.size() == 0;
}

bool JSONObject::empty_val_lists() const
{
    return key_to_value_list.size() == 0;
}

bool JSONObject::empty_obj_lists() const
{
    return key_to_object_list.size() == 0;
}

bool JSONObject::empty() const
{
    return key_to_value.size() == 0 &&
           key_to_object.size() == 0 &&
           key_to_value_list.size() == 0 &&
           key_to_object_list.size() == 0;
}

void JSONObject::set_value(std::string key, std::string& value)
{
    key_to_value[key] = value;
}

void JSONObject::set_value(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    start++;
    std::string value = pick_val(source, start);
    key_to_value[key] = value;
}

void JSONObject::set_object(std::string key, JSONObject& obj)
{
    key_to_object[key] = obj;
}

void JSONObject::set_object(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    JSONObject val(source, ++start);
    key_to_object[key] = val;
}

void JSONObject::set_obj_list(std::string key, std::list<JSONObject>& list)
{
    key_to_object_list[key] = list;
}

void JSONObject::set_obj_list(std::string &source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    std::list<JSONObject> val = pick_obj_list(source, ++start);
    key_to_object_list[key] = val;
}

void JSONObject::set_val_list(std::string key, std::list<std::string>& list)
{
    key_to_value_list[key] = list;
}

void JSONObject::set_val_list(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    std::list<std::string> val = pick_val_list(source, ++start);
    key_to_value_list[key] = val;
}

void JSONObject::erase_value(std::string key)
{
    key_to_value.erase(key);
}

void JSONObject::erase_object(std::string key)
{
    key_to_object.erase(key);
}

void JSONObject::erase_val_list(std::string key)
{
    key_to_value_list.erase(key);
}

void JSONObject::erase_obj_list(std::string key)
{
    key_to_object_list.erase(key);
}

void JSONObject::clear_values()
{
    key_to_value.clear();
}

void JSONObject::clear_objects()
{
    key_to_object.clear();
}

void JSONObject::clear_val_lists()
{
    key_to_value_list.clear();
}

void JSONObject::clear_obj_lists()
{
    key_to_object_list.clear();
}

void JSONObject::clear()
{
    key_to_value.clear();
    key_to_object.clear();
    key_to_value_list.clear();
    key_to_object_list.clear();
}

void JSONObject::read(std::string& path)
{
    std::ifstream in(path);
    if (!in.is_open())
        throw new std::invalid_argument("path invalid");

    std::string source;
    std::string line;
    while (std::getline(in, line))
        source += line;

    JSONObject tmp(source);
    key_to_value = tmp.get_name_to_value();
    key_to_object = tmp.get_name_to_object();
    key_to_value_list = tmp.get_name_to_values();
    key_to_object_list = tmp.get_name_to_objects();
}

void JSONObject::read_as_field(std::string& path)
{
    std::ifstream in(path);
    if (!in.is_open())
        throw new std::invalid_argument("path invalid");

    std::string source;
    std::string line;
    while (std::getline(in, line))
        source += line;

    size_t start = 0;
    std::string key = pick_val(source, start);
    JSONObject tmp(source, ++start);
    key_to_object[key] = tmp;
}

void JSONObject::read_and_overwrite(std::string& path)
{
    std::ifstream in(path);
    if (!in.is_open())
        throw new std::invalid_argument("path invalid");

    std::string source;
    std::string line;
    while (std::getline(in, line))
        source += line;

    JSONObject tmp(source);
    for (auto l : tmp.get_name_to_value())
        key_to_value[l.first] = l.second;

    for (auto l : tmp.get_name_to_object())
        key_to_object[l.first] = l.second;

    for (auto l : tmp.get_name_to_values())
        key_to_value_list[l.first] = l.second;

    for (auto l : tmp.get_name_to_objects())
        key_to_object_list[l.first] = l.second;

}

std::string JSONObject::format(std::string value)
{
    std::string res = "\"";
    for (int i = 0; i < value.size(); i++) {
        if (value[i] == '\"')
            res += "\\\"";
        else if (value[i] == '\'')
            res += "\\\'";
        else if (value[i] == '\t')
            res += "\\t";
        else if (value[i] == '\v')
            res += "\\v";
        else if (value[i] == '\b')
            res += "\\b";
        else if (value[i] == '\n')
            res += "\\n";
        else if (value[i] == '\\')
            res += "\\\\";
        else
            res += value[i];
    }

    res += "\"";
    return res;
}

std::string JSONObject::to_string(int tabs)
{
    std::string spacing(tabs, '\t');
    std::string res = "{\n";
    size_t counter = key_to_object.size() + key_to_value.size() + key_to_value_list.size() + key_to_object_list.size();
    for (auto l : key_to_value) {
        std::string key = spacing + '\t' + format(l.first);
        std::string val = format(l.second);
        res += key + ":" + val;
        if (counter > 1)
            res += ',';

        res += '\n';
        counter--;
    }

    for (auto l : key_to_value_list) {
        std::string key = spacing + '\t' + format(l.first);
        res += key + " : [";
        std::list<std::string>::iterator it = l.second.begin();
        if (it != l.second.end())
            res += format(*it);

        it++;
        while (it != l.second.end()) {
            res += ", " + format(*it);
        }

        counter--;
        res += "]";
        if (counter > 1)
            res += ',';
        res += '\n';
    }

    for (auto l : key_to_object) {

    }

    res += spacing + "}";
    return res;
}


