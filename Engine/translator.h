
#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "Json/json.hpp"


class Translator
{
    static bool empty;
    static nlohmann::json current_dict;
public:
    Translator();
    static void load(const char* path);
    static std::string translate(const char*);
};

#endif // TRANSLATOR_H
