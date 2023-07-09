
#ifndef JSON_H
#define JSON_H


#include<unordered_map>
#include<iostream>
#include<string>
#include <assert.h>

class JSONObject
{
private:
    std::unordered_map <std::string, JSONObject*>* name_to_object;
    std::unordered_map <std::string, std::vector<JSONObject*>* >* name_to_objects;
    std::unordered_map <std::string, std::vector<std::string>* >* name_to_values;
    std::unordered_map <std::string, std::string>* name_to_value;

    int find_block_end_1(std::string&, int);
    int find_block_end_2(std::string&, int);
public:
    JSONObject();
    ~JSONObject();
    JSONObject(std::string);
    JSONObject(const JSONObject&);

    JSONObject* get_object(std::string);
    std::vector<JSONObject*>* get_objects(std::string);
    std::vector<std::string>* get_values(std::string);
    std::string get_value(std::string);

    std::unordered_map <std::string, JSONObject*>* get_name_to_object();
    std::unordered_map <std::string, std::vector<JSONObject*>* >* get_name_to_objects();
    std::unordered_map <std::string, std::vector<std::string>* >* get_name_to_values();
    std::unordered_map <std::string, std::string>* get_name_to_value();

    bool is_in_values(std::string key);
    bool is_in_objects(std::string key);
    bool is_in_arrays(std::string key);
    bool is_in_object_arrays(std::string key);


    void operator =(const JSONObject&);

    void insert_object_array(std::string key, std::string array);
    void insert_array(std::string key, std::string array);
};





#endif // JSON_H
