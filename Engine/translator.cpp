
#include "translator.h"
#include <QFile>

Translator::Translator()
{
}

void Translator::load(const char *path)
{
    QFile file((QString(path)));
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen())
        throw new std::exception;

    current_dict = nlohmann::json::parse(file.readAll());
    file.close();
    empty = false;
}

std::string Translator::translate(const char * key)
{
    if (empty || !current_dict.contains(key))
        return key;

    return current_dict[key];
}

nlohmann::json Translator::current_dict;
bool Translator::empty;
