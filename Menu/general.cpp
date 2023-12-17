#include "general.h"

General* General::general = nullptr;

General::~General()
{
    delete general;
}

General *General::get_general()
{
    if (general == nullptr)
    {
        general = new General();
    }

    return general;
}

General::General(QObject *parent) : QObject(parent)
{

}
