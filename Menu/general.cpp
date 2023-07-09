#include "general.h"

General* General::general = 0;

General::~General()
{
    delete general;
}

General *General::get_general()
{
    if (!general)
        general = new General();
    return general;
}

General::General(QObject *parent) : QObject(parent)
{

}
